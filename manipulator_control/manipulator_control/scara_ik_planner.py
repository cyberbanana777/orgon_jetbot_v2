import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Pose2D
from std_msgs.msg import Float32, String
import math

class ScaraIKPlanner(Node):
    def __init__(self):
        super().__init__('scara_ik_planner')
        
        # Длины звеньев (м)
        self.declare_parameter('a1', 0.15)
        self.declare_parameter('a2', 0.10)
        # Конфигурация руки: +1 или -1
        self.declare_parameter('ARM', 1)
        
        self.a1 = self.get_parameter('a1').value
        self.a2 = self.get_parameter('a2').value
        self.ARM = self.get_parameter('ARM').value
        
        # Подписчик на целевые координаты
        self.sub = self.create_subscription(Pose2D, '/target_pose', self.pose_callback, 10)
        
        # Издатели углов и силы
        self.angle1_pub = self.create_publisher(Float32, '/angle1', 10)
        self.angle2_pub = self.create_publisher(Float32, '/angle2', 10)
        self.angle3_pub = self.create_publisher(Float32, '/angle3', 10)  # сила схвата
        
        # Издатель строковой команды для ESP32 или другого драйвера
        self.string_pub = self.create_publisher(String, '/scara_command_string', 10)
        
        self.get_logger().info(f'Scara IK Planner started: a1={self.a1}, a2={self.a2}, ARM={self.ARM}')
    
    def pose_callback(self, msg):
        x = msg.x
        y = msg.y
        dx = msg.theta  # сила схвата
        R = 0.043
        # Расчёт по формулам из Octave (адаптировано для atan2)
        r = math.sqrt(x*x + y*y)
        
        # Проверка достижимости
        if r < abs(self.a1 - self.a2) or r > (self.a1 + self.a2):
            self.get_logger().error(f'Target unreachable: distance {r:.3f} m is out of workspace')
            return
        
        # Угол от оси Y к вектору (x, y) — соответствует atan2(x, y)
        # Если в вашей системе координат Y вперёд, X вбок, то a = atan2(x, y)
        # Если X вперёд, Y вбок, используйте atan2(y, x). Уточните по моделированию.
        a = math.atan2(x, y)
        
        # Внутренние углы
        g1 = math.acos((self.a1**2 + r**2 - self.a2**2) / (2 * self.a1 * r))
        g2 = math.acos((self.a1**2 + self.a2**2 - r**2) / (2 * self.a1 * self.a2))
        
        q1 = -math.pi/2 + a + g1 * self.ARM
        q2 = (-math.pi + g2) * self.ARM
        
        # Нормализация углов (опционально)
        q1 = self.normalize_angle(q1)
        q2 = self.normalize_angle(q2)

        # Расчёт угла для схвата
        arg = dx/(2*R)
        if arg > 1:
            arg=1
        q3 = (8/6)*math.asin(arg)
        
        # Публикация отдельных значений
        self.publish_joints(q1, q2, q3)
        
        # Публикация строковой команды
        self.publish_string_command(q1, q2, q3)
        
        self.get_logger().info(f'Target (x={x:.3f}, y={y:.3f}) -> q1={math.degrees(q1):.1f}°, q2={math.degrees(q2):.1f}°, grip={math.degrees(q3):.1f}')
    
    def normalize_angle(self, angle):
        """Приведение угла к [-pi, pi]"""
        while angle > math.pi:
            angle -= 2.0 * math.pi
        while angle < -math.pi:
            angle += 2.0 * math.pi
        return angle
    
    def publish_joints(self, q1, q2, q3):
        msg1 = Float32()
        msg1.data = q1
        self.angle1_pub.publish(msg1)
        
        msg2 = Float32()
        msg2.data = q2
        self.angle2_pub.publish(msg2)
        
        msg3 = Float32()
        msg3.data = q3
        self.angle3_pub.publish(msg3)
    
    def publish_string_command(self, q1, q2, q3):
        """
        Формирует строку вида: $6;val1;val2;val3;#
        val1 = q1 (рад), val2 = q2 (рад), val3 = q3 (рад)
        """
        # Здесь можно добавить преобразование в градусы, если требуется драйверу
        # Но пока оставим в радианах, как в отдельных топиках
        cmd_str = f"$6;{math.degrees(q1):.0f};{math.degrees(q2):.0f};{math.degrees(q3):.0f};#"
        msg = String()
        msg.data = cmd_str
        self.string_pub.publish(msg)
        self.get_logger().debug(f'Published string command: {cmd_str}')

def main(args=None):
    rclpy.init(args=args)
    node = ScaraIKPlanner()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()