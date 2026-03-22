#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from geometry_msgs.msg import PoseWithCovarianceStamped
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose2D
import tf2_ros
from tf2_ros import TransformException
from geometry_msgs.msg import PointStamped
import tf2_geometry_msgs
import math
import sys
import time

class Brain(Node):

    def __init__(self):
        super().__init__('brain')
        
        self.current_pose = None
        self.current_goal = None
        self.sequence_index = 0
        self.position_tolerance = 0.05  # Допуск расхождения в метрах
        self.orientation_tolerance = 0.5  # Допуск расхождения в радианах

        # Переменные состояния манипулятора

        # Текущие
        self.x_manipulator = 0.0
        self.y_manipulator = 0.0
        self.theta_manipulator = 0.0
        self.is_grabbed = False

        # Стратегические
        self.manipulator_goal = None
        self.waiting_for_manipulator = False
        self.manipulator_position_tolerance = 0.01
        self.manipulator_orientation_tolerance = 0.1

        # TF2
        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)

        # Публикаторы

        # Публикатор целевой позиции NAV2
        self.robot_publisher = self.create_publisher(PoseStamped,
            '/goal_pose',
            10
        )

        # Публикатор в топик обратной кинематики манипулятора
        self.manipulator_publisher = self.create_publisher(
            Pose2D,
            '/hand_of_god/target_pose',
            10
        )

        # Подписчики
        
        # Подписка на координаты робота NAV2
        self.robot_subscription = self.create_subscription(
            PoseWithCovarianceStamped,
            '/amcl_pose',
            self.robot_pose_callback,
            10
        )
        
        # Подписка на координаты объектов c CV
        self.object_subscription = self.create_subscription(
            String,
            '/object_cords',
            self.object_pose_callback,
            10
        )

        # Таймер для проверки достижения цели роботом
        self.goal_check_timer = self.create_timer(1.0, self.check_goal_reached_timer)

        # Таймер для проверки достижения цели манипулятором
        self.manipulator_check_timer = self.create_timer(0.1, self.check_manipulator_goal)
        
        self.get_logger().info('Brain node initialized')



    # ========== Функции позиционирования РОБОТА ==========


    def publish_robot_goal(self, x, y, theta):
        """Публикация целевой точки"""

        msg = PoseStamped()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = 'map'
        
        # Позиция
        msg.pose.position.x = x
        msg.pose.position.y = y
        msg.pose.position.z = 0.0
        
        # Ориентация из угла theta (в радианах)
        msg.pose.orientation.z = math.sin(theta / 2.0)
        msg.pose.orientation.w = math.cos(theta / 2.0)
        
        self.robot_publisher.publish(msg)
        self.current_goal = msg.pose  # Сохраняем текущую цель для проверки
        self.get_logger().info(f'Опубликована цель: x={x}, y={y}, theta={theta:.2f}')


    def robot_pose_callback(self, msg):
        """Колбэк для получения текущей позиции робота"""

        self.current_pose = msg.pose.pose


    def check_goal_reached_timer(self):
        """Периодическая проверка достижения цели по таймеру"""
        if not self.waiting_for_goal_reached or not self.current_goal or not self.current_pose:
            return
                
        if self.is_robot_goal_reached():
            self.handle_goal_reached()


    def is_robot_goal_reached(self):
        """Проверка достижения цели по позиции и ориентации"""
        # Проверка позиции
        dx = self.current_goal.position.x - self.current_pose.position.x
        dy = self.current_goal.position.y - self.current_pose.position.y
        distance = math.sqrt(dx**2 + dy**2)
        
        # Проверка ориентации (yaw угол)
        current_yaw = self.quaternion_to_yaw(self.current_pose.orientation)
        goal_yaw = self.quaternion_to_yaw(self.current_goal.orientation)
        yaw_diff = abs(self.normalize_angle(current_yaw - goal_yaw))
        
        position_reached = distance < self.position_tolerance
        orientation_reached = yaw_diff < self.orientation_tolerance
        
        return position_reached and orientation_reached


    def quaternion_to_yaw(self, quat):
        """Преобразование кватерниона в угол yaw"""
        x, y, z, w = quat.x, quat.y, quat.z, quat.w
        yaw = math.atan2(2.0 * (w * z + x * y), 1.0 - 2.0 * (y * y + z * z))
        return yaw


    def normalize_angle(self, angle):
        """Нормализация угла в диапазон [-pi, pi]"""
        while angle > math.pi:
            angle -= 2.0 * math.pi
        while angle < -math.pi:
            angle += 2.0 * math.pi
        return angle


    def handle_goal_reached(self):
        """Обработка достижения цели"""
        self.get_logger().info('Цель достигнута!')
        self.waiting_for_goal_reached = False
        
        # цель манипулятора
        # self.publish_manipulator_goal(0.3, 0.1, 0.0)


    # ========== Функции позиционирования МАНИПУЛЯТОРА ==========


    def publish_manipulator_goal(self, x, y, theta=0.0):
        """Публикация целевой позиции для манипулятора"""
        pose_msg = Pose2D()
        pose_msg.x = x
        pose_msg.y = y
        pose_msg.theta = theta

        self.manipulator_publisher.publish(pose_msg)

        # сохраняем цель
        self.manipulator_goal = pose_msg
        self.waiting_for_manipulator = True

        self.get_logger().info(
            f'Отправлена цель манипулятору: x={x:.2f}, y={y:.2f}, theta={theta:.2f}'
        )


    def check_manipulator_goal(self):
        """Периодическая проверка достижения цели по таймеру"""
        if not self.waiting_for_manipulator or self.manipulator_goal is None:
            return

        self.update_manipulator_state()

        if self.is_manipulator_goal_reached():
            self.handle_manipulator_goal_reached()


    def is_manipulator_goal_reached(self):
        """Проверка достижекния цели манипулятором"""
        dx = self.manipulator_goal.x - self.x_manipulator
        dy = self.manipulator_goal.y - self.y_manipulator
        distance = math.sqrt(dx**2 + dy**2)

        dtheta = abs(self.normalize_angle(
            self.theta_manipulator - self.manipulator_goal.theta
        ))

        return (
            distance < self.manipulator_position_tolerance and
            dtheta < self.manipulator_orientation_tolerance
        )


    def update_manipulator_state(self):
        """Динамическое обновление позиции манипулятора"""
        if not self.waiting_for_manipulator or self.manipulator_goal is None:
            return

        alpha = 0.1  # скорость движения

        self.x_manipulator += alpha * (self.manipulator_goal.x - self.x_manipulator)
        self.y_manipulator += alpha * (self.manipulator_goal.y - self.y_manipulator)
        self.theta_manipulator += alpha * (
            self.manipulator_goal.theta - self.theta_manipulator
        )


    def handle_manipulator_goal_reached(self):
        """Обработка достижения цели манипулятором"""
        self.get_logger().info('Манипулятор достиг цели!')
        self.waiting_for_manipulator = False
        self.is_grabbed = True

        # дальнейшая логика взаимодействия



    # ========== Функции объектной логики ==========


    def target_pose_callback(self, msg): # это пока не точно
        """Получаем координаты объекта из CV и преобразуем в map"""

        try:
            parts = msg.data.strip().split()
            x_cam = float(parts[0])
            y_cam = float(parts[1])
            z_cam = float(parts[2])

            self.get_logger().info(
                f'CV (camera frame): x={x_cam}, y={y_cam}, z={z_cam}'
            )

            # преобразование camera → map
            result = self.transform_point(
                x_cam,
                y_cam,
                z_cam,
                from_frame='camera_link',  # ВАЖНО: имя фрейма камеры
                to_frame='map'
            )

            if result is None:
                return

            x_map, y_map, z_map = result

            self.get_logger().info(
                f'MAP: x={x_map:.2f}, y={y_map:.2f}, z={z_map:.2f}'
            )

            # 👉 можно отправить роботу или манипулятору
            self.publish_robot_goal(x_map, y_map, 0.0)

        except Exception as e:
            self.get_logger().warn(f'Ошибка парсинга CV: {e}')



    # ========== Функции transform ==========


    def transform_point(self, x, y, z, from_frame, to_frame='map'):
        """Преобразование точки между системами координат (video --> map) через tf2"""

        point = PointStamped()
        point.header.frame_id = from_frame
        point.header.stamp = self.get_clock().now().to_msg()

        point.point.x = x
        point.point.y = y
        point.point.z = z

        try:
            transform = self.tf_buffer.lookup_transform(
                to_frame,
                from_frame,
                rclpy.time.Time()
            )

            transformed_point = tf2_geometry_msgs.do_transform_point(point, transform)

            return (
                transformed_point.point.x,
                transformed_point.point.y,
                transformed_point.point.z
            )

        except TransformException as e:
            self.get_logger().warn(f'Ошибка TF: {e}')
            return None
    

    def base_to_manipulator(self, x, y, z):
        """Преобразование точки из base_link в систему манипулятора"""
        return self.transform_point(
            x, y, z,
            from_frame='base_link',
            to_frame='arm_base'
        )

    

            


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