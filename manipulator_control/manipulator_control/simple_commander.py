#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class MappingNode(Node):
    """
    ROS2 узел, который слушает топик 'input', сопоставляет полученное сообщение
    с записями в словаре и публикует соответствующее значение в топик 'output'.
    """

    def __init__(self):
        super().__init__('manipulator_control')

        # Словарь сопоставления (ключ -> значение)
        self.mapping_dict = {
            "low_opened": "$6;-45;30;90#",
            "low_closed": "$6;-45;30;0#",
            "high_closed": "$6;0;30;0#",
            "high_opened": "$6;0;30;90#"
        }

        # Подписчик на топик 'input' (тип String)
        self.subscription = self.create_subscription(
            String,
            '/manipulator_control',
            self.listener_callback,
            10
        )
        # Публикатор в топик 'output' (тип String)
        self.publisher = self.create_publisher(String, '/low_level/serial/cmd', 10)

        self.get_logger().info("Mapping node started. Listening on '/manipulator_control', publishing on '/low_level/serial/cmd'.")

    def listener_callback(self, msg):
        """
        Callback, вызываемый при получении сообщения.
        Ищет полученную строку в словаре и публикует соответствующее значение.
        """
        received_str = msg.data
        self.get_logger().info(f"Received: '{received_str}'")

        # Поиск в словаре
        if received_str in self.mapping_dict:
            output_str = self.mapping_dict[received_str]
            self.get_logger().info(f"Mapping found: '{received_str}' -> '{output_str}'")
        else:
            output_str = f"Unknown: {received_str}"
            self.get_logger().warn(f"No mapping for '{received_str}'. Publishing: '{output_str}'")

        # Публикация результата
        out_msg = String()
        out_msg.data = output_str
        self.publisher.publish(out_msg)

def main(args=None):
    rclpy.init(args=args)
    node = MappingNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()