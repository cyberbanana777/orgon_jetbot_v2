#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from std_msgs.msg import String
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav2_msgs.action import NavigateToPose
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose2D
import math
import sys

class Brain(Node):
    def __init__(self):
        super().__init__('Brain')  # Имя узла в ROS2

        # Переменные для хранения текущего состояния навигации
        self.current_goal_sent = False
        self.current_pose = None

        # Переменные для хранения текущего состояния манипулятора
        self.x_manipulator = 0
        self.y_manipulator = 0
        self.is_grabbed = False

        # Создаем Action Client для /navigate_to_pose
        self._action_client = ActionClient(
            self,
            NavigateToPose,
            'navigate_to_pose'
        )

        # Параметры: тип сообщения, имя топика, размер очереди
        self.pose_publisher = self.create_publisher(
            Pose2D,
            '\hand_of_god/target_pose', # топик принимающий IK
            10)
        
        # Подписка на координаты робота
        self.pose_subscription = self.create_subscription(
            PoseWithCovarianceStamped,
            '/amcl_pose',
            self.robot_pose_callback,
            10
        )
        
        # Подписка на координаты объектов
        self.object_subscription = self.create_subscription(
            String,
            '/object_cords',
            self.target_pose_callback,
            10
        )
        
        self.get_logger().info('Brain node initialized')
    
    def pose_publisher(self,)

    def send_goal(self, x, y, yaw_radians):
        # Проверяем, не отправлена ли уже цель
        if self.current_goal_sent:
            self.get_logger().warn('Цель уже отправлена, дождитесь завершения')
            return
        
        # Ждем, пока Action-сервер станет доступен
        if not self._action_client.wait_for_server(timeout_sec=1.0):
            self.get_logger().error('Action сервер не доступен')
            return
        
        # Формируем цель
        goal_msg = NavigateToPose.Goal()
        goal_msg.pose.header.frame_id = 'map'
        goal_msg.pose.header.stamp = self.get_clock().now().to_msg()
        
        # Задаем позицию
        goal_msg.pose.pose.position.x = x
        goal_msg.pose.pose.position.y = y
        goal_msg.pose.pose.position.z = 0.0
        
        # Конвертируем угол в кватернион
        goal_msg.pose.pose.orientation.x = 0.0
        goal_msg.pose.pose.orientation.y = 0.0
        goal_msg.pose.pose.orientation.z = math.sin(yaw_radians / 2.0)
        goal_msg.pose.pose.orientation.w = math.cos(yaw_radians / 2.0)
        
        self.get_logger().info(f'Отправка цели: x={x:.2f}, y={y:.2f}, угол={yaw_radians:.2f} рад')
        
        # Отправляем цель
        self.current_goal_sent = True
        self._send_goal_future = self._action_client.send_goal_async(
            goal_msg,
            feedback_callback=self.feedback_callback
        )
        self._send_goal_future.add_done_callback(self.goal_response_callback)
    
    def goal_response_callback(self, future):
        goal_handle = future.result()
        if not goal_handle.accepted:
            self.get_logger().error('Цель отклонена!')
            self.current_goal_sent = False
            return
        
        self.get_logger().info('Цель принята, начинаю навигацию...')
        self._get_result_future = goal_handle.get_result_async()
        self._get_result_future.add_done_callback(self.result_callback)
    
    def feedback_callback(self, feedback_msg):
        # Получаем обратную связь о прогрессе
        feedback = feedback_msg.feedback
        # Раскомментируйте для отладки
        # if hasattr(feedback, 'distance_remaining'):
        #     self.get_logger().info(f'Осталось: {feedback.distance_remaining:.2f} м')
        pass
    
    def result_callback(self, future):
        result = future.result().result
        self.get_logger().info('Навигация завершена!')
        self.current_goal_sent = False
        
        # Здесь можно добавить логику после достижения цели
        # Например, отправить следующую цель или выполнить действие
    
    def robot_pose_callback(self, msg: PoseWithCovarianceStamped):
        # Сохраняем текущую позицию
        self.current_pose = msg
        
        x = msg.pose.pose.position.x
        y = msg.pose.pose.position.y
        
        # Получаем ориентацию (рыскание)
        qz = msg.pose.pose.orientation.z
        qw = msg.pose.pose.orientation.w
        yaw = 2.0 * math.atan2(qz, qw)
        
        self.get_logger().info(f'Текущая позиция робота: x={x:.2f}, y={y:.2f}, угол={yaw:.2f} рад')
    
    def target_pose_callback(self, msg: String):
        self.get_logger().info(f'Получены координаты объекта: {msg.data}')
        
        # Парсим полученные данные

def main(args=None):
    rclpy.init(args=args)
    node = Brain()
    
    # Логика навигации робота

    # Логика поведения манипулятора
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Узел остановлен пользователем')
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()