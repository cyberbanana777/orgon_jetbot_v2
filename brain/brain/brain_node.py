import math
import rclpy
from rclpy.node import Node
from rclpy.time import Time
from rclpy.duration import Duration
from tf2_ros import TransformException, Buffer, TransformListener
import tf2_geometry_msgs
from geometry_msgs.msg import PointStamped, Twist, PoseStamped
from nav2_msgs.action import NavigateToPose
from rclpy.action import ActionClient
from std_msgs.msg import Int32, String

class Brain(Node):
    def __init__(self):
        super().__init__('brain_node')

        # --- Переменные состояния ---
        self.state = 'IDLE'           # состояния: IDLE, NAV_TO_OBJECT, ALIGN_TO_OBJECT, AT_OBJECT,
                                      # MANIP_GRAB, GRABBING, NAV_TO_BASKET, ALIGN_TO_BASKET, AT_BASKET,
                                      # MANIP_PLACE, PLACING

        self.target_index = -1        # индекс текущей цели
        self.target_coords = None     # координаты объекта (x, y) в метрах
        self.nav_goal = None          # целевая точка для навигации

        self.basket_coords = (1.125, 0.125)   # центр корзинки (метры)

        # Параметры смещений для подъезда
        self.object_offset_distance = 0.025    # отступ назад (2.5 см)
        self.object_offset_right = 0.04        # смещение вправо (4 см)
        self.basket_offset_distance = 0.025    # отступ от корзинки
        self.basket_offset_right = 0.04        # смещение вправо для корзинки

        # Флаги навигации и манипулятора
        self.robot_reached_goal = False       # устанавливается в handle_goal_reached
        self.robot_going_to_goal = False
        self.manipulator_going_to_goal = False
        self.manipulator_is_grabbed = False

        # --- Для поворота ---
        self.cmd_vel_pub = self.create_publisher(Twist, '/cmd_vel', 10)
        self.alignment_active = False
        self.alignment_target_yaw = 0.0
        self.alignment_tolerance = 0.05      # радиан (~3°)
        self.k_angular = 1.0
        self.max_angular = 0.5               # рад/с

        # --- Текущая поза (из amcl) ---
        self.current_pose = None

        # --- Клиент навигации (Action) ---
        self.nav_action_client = ActionClient(self, NavigateToPose, '/navigate_to_pose')

        # --- Публикация команд манипулятору (заглушка) ---
        self.manip_pub = self.create_publisher(Int32, '/manipulator_goal', 10)

        # --- Публикация команд схвату (заглушка) ---
        self.grip_pub = self.create_publisher(String, '/grip_command', 10)

        # --- Подписка на AMCL позу ---
        self.amcl_sub = self.create_subscription(PoseStamped, '/amcl_pose', self.amcl_pose_callback, 10)

        # --- Таймер для цикла управления (10 Гц) ---
        self.control_timer = self.create_timer(0.1, self.control_cycle)

        # --- TF буфер для преобразований (если понадобится) ---
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        self.get_logger().info('Brain node initialized')

    # ========== Навигация ==========
    def publish_robot_goal(self, x, y, yaw=0.0):
        """Отправить цель в навигационный Action."""
        goal_msg = NavigateToPose.Goal()
        goal_msg.pose.header.frame_id = 'map'
        goal_msg.pose.header.stamp = self.get_clock().now().to_msg()
        goal_msg.pose.pose.position.x = x
        goal_msg.pose.pose.position.y = y
        goal_msg.pose.pose.orientation.z = math.sin(yaw / 2.0)
        goal_msg.pose.pose.orientation.w = math.cos(yaw / 2.0)

        self.nav_action_client.wait_for_server()
        self._send_goal_future = self.nav_action_client.send_goal_async(goal_msg)
        self._send_goal_future.add_done_callback(self.goal_response_callback)

    def goal_response_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().error('Цель навигации отклонена')
            return
        self.get_logger().info('Цель навигации принята')
        self._result_future = goal_handle.get_result_async()
        self._result_future.add_done_callback(self.goal_result_callback)

    def goal_result_callback(self, future):
        result = future.result().result
        if result:
            self.get_logger().info('Навигация завершена')
            self.robot_reached_goal = True
        else:
            self.get_logger().error('Навигация не удалась')
            self.robot_reached_goal = False

    # ========== Манипулятор ==========
    def send_manipulator_goal(self, pose_id: int):
        """Отправить цель манипулятору (позиция 1,2,3)."""
        msg = Int32()
        msg.data = pose_id
        self.manip_pub.publish(msg)
        self.get_logger().info(f'Отправлена цель манипулятору: {pose_id}')
        self.manipulator_going_to_goal = True

    def handle_manipulator_goal_reached(self):
        """Обработка достижения цели манипулятором (вызывается из подписки)."""
        self.get_logger().info('Манипулятор достиг цели')
        self.manipulator_going_to_goal = False

    # ========== Схват ==========
    def grip_close(self):
        msg = String()
        msg.data = 'close'
        self.grip_pub.publish(msg)
        self.get_logger().info('Схват сжат')

    def grip_open(self):
        msg = String()
        msg.data = 'open'
        self.grip_pub.publish(msg)
        self.get_logger().info('Схват разжат')

    # ========== Вычисление позиций ==========
    def calc_nav_position(self, obj_x, obj_y):
        """Вычисляет навигационную цель для подъезда к объекту."""
        target_x = obj_x + self.object_offset_right
        target_y = obj_y - self.object_offset_distance
        return target_x, target_y

    def calc_basket_nav_position(self):
        """Вычисляет навигационную цель для подъезда к корзинке."""
        bx, by = self.basket_coords
        target_x = bx + self.basket_offset_right
        target_y = by - self.basket_offset_distance
        return target_x, target_y

    def set_target(self):
        """Выбирает следующую цель из списка, возвращает (x, y) в метрах."""
        # Координаты в метрах (были в см: 22,52 => 0.22,0.52 и т.д.)
        targets = [
            (0.22, 0.52),
            (0.52, 0.52),
            (0.73, 0.52),
            (0.27, 0.77),
            (0.48, 0.77)
        ]
        if self.target_index < len(targets) - 1:
            self.target_index += 1
        else:
            self.target_index = 0   # зацикливание
        self.get_logger().info(f'Выбрана цель {self.target_index}: {targets[self.target_index]}')
        return targets[self.target_index]

    # ========== Управление поворотом ==========
    def get_yaw_from_pose(self, pose):
        """Извлекает угол рыскания из кватерниона."""
        q = pose.orientation
        siny_cosp = 2.0 * (q.w * q.z + q.x * q.y)
        cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z)
        return math.atan2(siny_cosp, cosy_cosp)

    def normalize_angle(self, angle):
        while angle > math.pi:
            angle -= 2.0 * math.pi
        while angle < -math.pi:
            angle += 2.0 * math.pi
        return angle

    def align_to_point(self, target_x, target_y):
        """Начать выравнивание робота лицом к заданной точке."""
        if self.current_pose is None:
            self.get_logger().warn('Нет текущей позиции для выравнивания')
            return False
        dx = target_x - self.current_pose.position.x
        dy = target_y - self.current_pose.position.y
        self.alignment_target_yaw = math.atan2(dy, dx)
        self.alignment_active = True
        self.get_logger().info(f'Начато выравнивание к углу {self.alignment_target_yaw:.2f}')
        return True

    def stop_robot(self):
        self.cmd_vel_pub.publish(Twist())

    def update_alignment(self):
        """Выполняет один шаг поворота. Возвращает True, если цель достигнута."""
        if not self.alignment_active:
            return True
        if self.current_pose is None:
            return False

        current_yaw = self.get_yaw_from_pose(self.current_pose)
        delta = self.normalize_angle(self.alignment_target_yaw - current_yaw)

        if abs(delta) < self.alignment_tolerance:
            self.alignment_active = False
            self.stop_robot()
            self.get_logger().info('Выравнивание завершено')
            return True

        angular = self.k_angular * delta
        angular = max(min(angular, self.max_angular), -self.max_angular)

        twist = Twist()
        twist.angular.z = angular
        self.cmd_vel_pub.publish(twist)
        return False

    # ========== Колбэки ==========
    def amcl_pose_callback(self, msg: PoseStamped):
        self.current_pose = msg.pose

    def target_pose_callback(self, msg):
        """Получаем координаты объекта из CV и преобразуем в map (заглушка)."""
        try:
            parts = msg.data.strip().split()
            x_cam = float(parts[0])
            y_cam = float(parts[1])
            z_cam = float(parts[2])
            self.get_logger().info(f'CV (camera frame): x={x_cam}, y={y_cam}, z={z_cam}')
            # Здесь можно преобразовать через transform_point и отправить роботу
        except Exception as e:
            self.get_logger().warn(f'Ошибка парсинга CV: {e}')

    # ========== Главный цикл управления (конечный автомат) ==========
    def control_cycle(self):
        """Вызывается по таймеру 10 Гц."""
        # Если идёт выравнивание – занимаемся только им
        if self.alignment_active:
            self.update_alignment()
            return

        # --- Состояние IDLE: выбор цели и начало навигации ---
        if self.state == 'IDLE':
            if self.target_index == -1:
                self.target_coords = self.set_target()
            else:
                self.target_coords = self.set_target()
            self.nav_goal = self.calc_nav_position(self.target_coords[0], self.target_coords[1])
            self.get_logger().info(f'Навигация к объекту: {self.nav_goal}')
            self.publish_robot_goal(self.nav_goal[0], self.nav_goal[1])
            self.state = 'NAV_TO_OBJECT'
            self.robot_going_to_goal = True

        # --- Состояние NAV_TO_OBJECT: ожидаем прибытия к объекту ---
        if self.state == 'NAV_TO_OBJECT' and self.robot_reached_goal:
            self.get_logger().info('Робот достиг позиции у объекта')
            self.robot_reached_goal = False
            self.robot_going_to_goal = False
            if self.align_to_point(self.target_coords[0], self.target_coords[1]):
                self.state = 'ALIGN_TO_OBJECT'
            else:
                self.state = 'AT_OBJECT'   # fallback

        # --- Состояние ALIGN_TO_OBJECT: завершение выравнивания ---
        if self.state == 'ALIGN_TO_OBJECT' and not self.alignment_active:
            self.state = 'AT_OBJECT'

        # --- Состояние AT_OBJECT: захват объекта ---
        if self.state == 'AT_OBJECT' and not self.manipulator_is_grabbed:
            self.send_manipulator_goal(1)   # позиция захвата
            self.state = 'MANIP_GRAB'

        # --- Состояние MANIP_GRAB: ожидание движения манипулятора, затем захват ---
        if self.state == 'MANIP_GRAB' and not self.manipulator_going_to_goal:
            self.grip_close()
            self.manipulator_is_grabbed = True
            self.send_manipulator_goal(3)   # сложить манипулятор
            self.state = 'GRABBING'

        # --- Состояние GRABBING: после складывания – едем к корзинке ---
        if self.state == 'GRABBING' and not self.manipulator_going_to_goal:
            basket_goal = self.calc_basket_nav_position()
            self.publish_robot_goal(basket_goal[0], basket_goal[1])
            self.state = 'NAV_TO_BASKET'
            self.robot_going_to_goal = True

        # --- Состояние NAV_TO_BASKET: ожидаем прибытия к корзинке ---
        if self.state == 'NAV_TO_BASKET' and self.robot_reached_goal:
            self.get_logger().info('Робот достиг корзинки')
            self.robot_reached_goal = False
            self.robot_going_to_goal = False
            if self.align_to_point(self.basket_coords[0], self.basket_coords[1]):
                self.state = 'ALIGN_TO_BASKET'
            else:
                self.state = 'AT_BASKET'

        # --- Состояние ALIGN_TO_BASKET: завершение выравнивания ---
        if self.state == 'ALIGN_TO_BASKET' and not self.alignment_active:
            self.state = 'AT_BASKET'

        # --- Состояние AT_BASKET: разгрузка объекта ---
        if self.state == 'AT_BASKET' and self.manipulator_is_grabbed:
            self.send_manipulator_goal(2)   # позиция разгрузки
            self.state = 'MANIP_PLACE'

        # --- Состояние MANIP_PLACE: ожидание движения манипулятора, затем разжать ---
        if self.state == 'MANIP_PLACE' and not self.manipulator_going_to_goal:
            self.grip_open()
            self.manipulator_is_grabbed = False
            self.send_manipulator_goal(3)   # сложить манипулятор
            self.state = 'PLACING'

        # --- Состояние PLACING: после складывания – возвращаемся в IDLE для следующей цели ---
        if self.state == 'PLACING' and not self.manipulator_going_to_goal:
            self.state = 'IDLE'
            self.robot_reached_goal = False
            self.get_logger().info('Цикл завершён, переход к следующему объекту')


def main(args=None):
    rclpy.init(args=args)
    node = Brain()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Узел остановлен пользователем')
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()