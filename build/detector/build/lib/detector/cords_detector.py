#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy
from sensor_msgs.msg import CameraInfo, CompressedImage  # CameraInfo нужен для внутренних параметров
from aruco_msgs.msg import MarkerArray
from std_msgs.msg import String
import numpy as np
import tf_transformations as tf

class ObjectLocalizer(Node):
    def __init__(self):
        super().__init__('object_localizer')

        # ---------- Параметры ----------
        self.declare_parameter('grid_origin_x', 0.0)    # начало сетки меток по X (м)
        self.declare_parameter('grid_origin_y', 0.0)    # начало сетки меток по Y (м)
        self.declare_parameter('cell_size', 0.1)        # размер клетки (м)
        self.declare_parameter('grid_width', 5)         # количество меток по X
        self.declare_parameter('grid_height', 5)        # количество меток по Y

        self.origin_x = self.get_parameter('grid_origin_x').value
        self.origin_y = self.get_parameter('grid_origin_y').value
        self.cell_size = self.get_parameter('cell_size').value
        self.grid_width = self.get_parameter('grid_width').value
        self.grid_height = self.get_parameter('grid_height').value

        # Внутренние параметры камеры (будут получены из CameraInfo)
        self.fx = self.fy = self.cx = self.cy = None
        self.camera_info_received = False

        # Поза камеры в мировой системе (пол Z=0)
        self.camera_pose_valid = False
        self.R_cw = np.eye(3)   # поворот камера -> мир
        self.t_cw = np.zeros(3) # положение камеры в мире

        # ---------- Подписки ----------
        qos = QoSProfile(depth=10, reliability=QoSReliabilityPolicy.BEST_EFFORT)

        # Camera info (для получения fx, fy, cx, cy)
        self.cam_info_sub = self.create_subscription(
            CameraInfo, '/camera/camera/color/camera_info', self.camera_info_callback, qos)

        # Aruco метки (поза камеры)
        self.aruco_sub = self.create_subscription(
            MarkerArray, '/aruco/cords', self.aruco_callback, qos)

        # Детектированные объекты (строки)
        self.detected_sub = self.create_subscription(
            String, '/detected_objects', self.objects_callback, qos)

        # ---------- Публикатор ----------
        self.objects_pub = self.create_publisher(String, '/objects_cords', 10)

        self.get_logger().info('Object localizer ready, waiting for camera info and aruco markers...')

    def camera_info_callback(self, msg: CameraInfo):
        """Получаем матрицу камеры из CameraInfo."""
        if self.camera_info_received:
            return  # достаточно одного сообщения
        K = np.array(msg.k).reshape(3, 3)
        self.fx = K[0, 0]
        self.fy = K[1, 1]
        self.cx = K[0, 2]
        self.cy = K[1, 2]
        self.camera_info_received = True
        self.get_logger().info(f'Camera intrinsics: fx={self.fx}, fy={self.fy}, cx={self.cx}, cy={self.cy}')

    def aruco_callback(self, msg: MarkerArray):
        """Оцениваем позу камеры по первому подходящему маркеру."""
        if not self.camera_info_received:
            self.get_logger().warn('Camera info not yet received, skipping aruco pose estimation')
            return

        for marker in msg.markers:
            marker_id = marker.id
            # Проверяем, что ID в пределах сетки
            if marker_id < 0 or marker_id >= self.grid_width * self.grid_height:
                continue

            # Реальные координаты маркера в мире (Z=0)
            i = marker_id // self.grid_width
            j = marker_id % self.grid_width
            x_w = self.origin_x + i * self.cell_size
            y_w = self.origin_y + j * self.cell_size
            t_w = np.array([x_w, y_w, 0.0])

            # Поза маркера относительно камеры (из сообщения)
            pos = marker.pose.pose.position
            ori = marker.pose.pose.orientation
            t_m = np.array([pos.x, pos.y, pos.z])
            q_m = [ori.x, ori.y, ori.z, ori.w]
            R_m = tf.quaternion_matrix(q_m)[:3, :3]   # поворот маркера -> камера

            # Поза камеры в мире: R_cw = R_m, t_cw = t_m - R_m * t_w
            self.R_cw = R_m
            self.t_cw = t_m - R_m @ t_w
            self.camera_pose_valid = True

            self.get_logger().debug(f'Camera pose from marker {marker_id}: t={self.t_cw}')
            break   # используем только первый маркер

    def objects_callback(self, msg: String):
        """Обрабатываем строку из детектора и публикуем координаты объекта."""
        if not self.camera_pose_valid:
            self.get_logger().warn('Camera pose not yet available, skipping object')
            return

        if not self.camera_info_received:
            self.get_logger().warn('Camera intrinsics not yet available, skipping object')
            return

        # Парсим строку: "shape, color, u, v"
        parts = msg.data.split(', ')
        if len(parts) != 4:
            self.get_logger().warn(f'Invalid message format: {msg.data}')
            return
        shape, color = parts[0], parts[1]
        try:
            u = int(parts[2])
            v = int(parts[3])
        except ValueError:
            self.get_logger().warn(f'Could not parse coordinates: {parts[2]}, {parts[3]}')
            return

        # Нормализованные координаты в системе камеры (луч)
        x_cam = (u - self.cx) / self.fx
        y_cam = (v - self.cy) / self.fy
        v_cam = np.array([x_cam, y_cam, 1.0])

        # Луч в мировой системе
        v_world = self.R_cw @ v_cam

        # Находим пересечение с плоскостью Z=0
        if abs(v_world[2]) < 1e-6:
            self.get_logger().warn('Ray is parallel to floor, cannot compute intersection')
            return
        t = -self.t_cw[2] / v_world[2]
        if t < 0:
            self.get_logger().warn('Intersection behind camera')
            return

        # Мировые координаты объекта
        P = self.t_cw + t * v_world
        P[2] = 0.0   # принудительно на пол

        # Формируем строку для публикации: "shape, color, x, y, z"
        out_str = f"{shape}, {color}, {P[0]:.3f}, {P[1]:.3f}, {P[2]:.3f}"
        self.objects_pub.publish(String(data=out_str))
        self.get_logger().info(f'Published: {out_str}')

def main(args=None):
    rclpy.init(args=args)
    node = ObjectLocalizer()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()