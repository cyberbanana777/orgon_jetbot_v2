#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from std_msgs.msg import Float32
from sensor_msgs.msg import JointState

class JointStateBridge(Node):
    def __init__(self):
        super().__init__('joint_state_bridge')

        self.q1 = 0.0
        self.q2 = 0.0
        self.q3 = 0.0
        self.q4 = 0.0
        self.q5 = 0.0

        self.create_subscription(Float32, '/hand_of_god/angle1', self.cb1, 10)
        self.create_subscription(Float32, '/hand_of_god/angle2', self.cb2, 10)
        self.create_subscription(Float32, '/hand_of_god/angle3', self.cb3, 10)

        self.pub = self.create_publisher(JointState, '/joint_states', 10)

        # постоянная публикация
        self.timer = self.create_timer(0.05, self.publish_joint_states)

    def cb1(self, msg): self.q1 = msg.data
    def cb2(self, msg): self.q2 = msg.data
    def cb3(self, msg): self.q3 = msg.data

    def publish_joint_states(self):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()

        msg.name = [
            'base_first_link_joint',
            'first_second_links_joint',
            'platform_left_finger_link_joint',
            'platform_right_finger_link_joint',
            'base_link_left_wheel_joint',
            'base_link_right_wheel_joint'
        ]

        msg.position = [
            self.q1,
            self.q2,
            self.q3,
            -self.q3,
            self.q4,
            self.q5
        ]

        self.pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)

    node = JointStateBridge()
    node.get_logger().info("JointStateBridge started")

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()