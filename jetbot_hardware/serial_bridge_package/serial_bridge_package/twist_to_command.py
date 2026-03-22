
#!/usr/bin/env python3

# Copyright (c) 2025 Alice Zenina and Alexander Grachev RTU MIREA (Russia)
# SPDX-License-Identifier: MIT
# Details in the LICENSE file in the root of the package.

'''
АННОТАЦИЯ
Конвертер сообщений Twist в кастомный протокол для ESP32 с форматом
$linear;angular#. Извлекает только линейную скорость по X и угловую
по Z, игнорируя остальные компоненты. Использует стандартный топик
/cmd_vel для совместимости с ROS-навигационными стеками.

ANNOTATION
Twist message converter to custom ESP32 protocol in $linear;angular# format.
Extracts only linear X and angular Z velocities while ignoring other
components. Uses standard /cmd_vel topic for compatibility with ROS navigation
stacks.
'''

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_msgs.msg import String
from std_msgs.msg import Bool


class TwistToCommand(Node):
    """Node for converting Twist messages to custom ESP32 command protocol."""

    def __init__(self):
        super().__init__('twist_to_command')
        
        # Состояние Aruco (по умолчанию False)
        self.aruco_flag = False

        # Subscription to Twist topic (movement control)
        self.subscription = self.create_subscription(
            Twist,
            '/cmd_vel',  # Standard topic for movement control
            self.twist_callback,
            10
        )

        # Subscription to Aruco State topic (vision)
        self.aruco_sub = self.create_subscription(
            Bool,
            '/aruco_state',
            self.aruco_callback,
            10
        )
        
        # Publisher for ESP32 communication topic
        self.publisher = self.create_publisher(
            String,
            '/esp32_input',  # Topic used for sending data to ESP32
            10
        )

        # Таймер, по умолчанию неактивен
        self.timer = None
        
        self.get_logger().info(
            'Twist-to-command node started. Waiting for Twist commands...'
        )

    def aruco_callback(self, msg: Bool):
        """Обработка состояния Aruco. Триггер один раз."""
        if msg.data:  # получили True
            self.get_logger().info('Aruco flag received True. Starting 90s timer.')
            self.aruco_flag = True

            # Отключаем подписку, больше не интересуемся топиком
            self.destroy_subscription(self.aruco_sub)
            self.aruco_sub = None

            # Запускаем таймер на 90 секунд
            self.timer = self.create_timer(89.5, self.timer_callback) # не 90, что бы наверняка не выйти за пределы

    def timer_callback(self):
        """Срабатывает после 90 секунд."""
        self.get_logger().info('90 seconds passed. Aruco flag set to False.')
        self.aruco_flag = False
        # Останавливаем таймер
        if self.timer is not None:
            self.timer.cancel()
            self.timer = None

    def twist_callback(self, msg):
        """Convert Twist message to ESP32 command string."""

        # Публикуем команду в зависимости от aruco_flag.
        if self.aruco_flag:
            linear_x = msg.linear.x
            angular_z = msg.angular.z
        else:
            linear_x = 0.0
            angular_z = 0.0
        
        # Format message according to protocol
        command = f"$1;{linear_x:.3f};{angular_z:.3f};#"
        
        # Create and publish String message
        string_msg = String()
        string_msg.data = command
        
        self.publisher.publish(string_msg)
        self.get_logger().debug(f'Sent command: {command}')


def main(args=None):
    """Main function to initialize and run the node."""
    rclpy.init(args=args)
    node = TwistToCommand()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Twist-to-command node stopped')
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
