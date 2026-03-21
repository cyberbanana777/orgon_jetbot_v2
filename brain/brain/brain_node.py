#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from geometry_msgs.msg import PoseWithCovarianceStamped
import sys

class SimpleNode(Node):
    def __init__(self):
        super().__init__('simple_node')
        
        # Подписка на координаты робота
        self.subscription = self.create_subscription(
            PoseWithCovarianceStamped,
            '/amcl_pose',
            self.robot_pose_collback,
            10
        )

        # Подписка на координаты объектов
        self.subscription = self.create_subscription(
            String,
            '/object_cords',
            self.target_pose_collback,
            10
        )
        
        self.get_logger().info('Brain for Jetbot')

    def robot_pose_collback(self, msg: PoseWithCovarianceStamped):
        self.get_logger().info(f'Received: {msg.data}')


    def target_pose_collback(self, msg: String):
        self.get_logger().info(f'Received: {msg.data}')

def main(args=None):
    rclpy.init(args=args)
    node = SimpleNode()


    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()