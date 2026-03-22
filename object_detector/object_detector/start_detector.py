import rclpy
from rclpy.node import Node
from aruco_msgs.msg import ArucoMarkers
from std_msgs.msg import Bool

class ArucoStartNode(Node):
    def __init__(self):
        super().__init__('start_detector')
        self.subscription = self.create_subscription(
            ArucoMarkers,
            '/aruco/markers',
            self.aruco_callback,
            10)
        self.publisher = self.create_publisher(Bool, '/aruco_start', 10)
        self.get_logger().info('Aruco start node initialized')

    def aruco_callback(self, msg: ArucoMarkers):
        # Проверяем, есть ли маркер с id=6
        marker_present = any(marker.id == 6 for marker in msg.markers)
        # Создаем сообщение Bool
        pub_msg = Bool()
        pub_msg.data = marker_present
        self.publisher.publish(pub_msg)
        self.get_logger().info(f'Published /aruco_start: {marker_present}')

def main(args=None):
    rclpy.init(args=args)
    node = ArucoStartNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()