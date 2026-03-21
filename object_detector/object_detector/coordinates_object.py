import rclpy
from rclpy.node import Node
from sensor_msgs.msg import CameraInfo
from aruco_msgs.msg import ArucoMarkers
from std_msgs.msg import String
import numpy as np

class ObjectLocalization(Node):
    def __init__(self):
        super().__init__('object_localization')

        # Subscribers
        self.aruco_sub = self.create_subscription(
            ArucoMarkers,
            '/aruco/markers',
            self.aruco_callback,
            10)

        self.detected_objects_sub = self.create_subscription(
            String,
            '/detected_objects',
            self.detected_objects_callback,
            10)

        # Publisher for object coordinates in the required format
        self.objects_pub = self.create_publisher(
            String,
            '/object_cords',
            10)

        # Хранилище для меток: каждая метка – (u, v, x, y, z)
        self.markers = []  # список кортежей

        # Параметры камеры (не используются в этом варианте)
        self.camera_matrix = None

    def camera_info_callback(self, msg: CameraInfo):
        self.camera_matrix = np.array(msg.k).reshape(3, 3)
        self.get_logger().info("Camera info received")

    def aruco_callback(self, msg: ArucoMarkers):
        for marker in msg.markers:
            # Пиксельные координаты центра метки (2D)
            u = marker.center.x
            v = marker.center.y
            # Реальные координаты центра метки (3D) в системе камеры
            x = marker.pose.position.x
            y = marker.pose.position.y
            z = marker.pose.position.z
            
            if marker.id == 20 or marker.id == 21:
                continue

            self.markers.append((u, v, x, y, z))
            self.get_logger().info(f"Marker: id={marker.id}, u={u:.1f}, v={v:.1f}, x={x:.3f}, y={y:.3f}, z={z:.3f}")

        # Ограничиваем размер
        if len(self.markers) > 100:
            self.markers.pop(0)

    def detected_objects_callback(self, msg: String):
        # Парсим строку: "тип, цвет, u, v"
        parts = msg.data.split(',')
        if len(parts) != 4:
            return
        obj_type = parts[0].strip()
        obj_color = parts[1].strip()
        try:
            u = int(parts[2].strip())
            v = int(parts[3].strip())
        except ValueError:
            return

        point = self.interpolate_object_coords(u, v)
        if point is None:
            return

        # Формируем строку: тип, цвет, x, y, z
        result_str = f"{obj_type},{obj_color},{point[0]:.3f},{point[1]:.3f},{point[2]:.3f}"
        self.objects_pub.publish(String(data=result_str))
        self.get_logger().info(f"Published: {result_str}")

    def interpolate_object_coords(self, u, v, k=3):
        """
        Возвращает реальные координаты объекта (x,y,z) на основе k ближайших меток.
        Использует обратно-взвешенное расстояние (inverse distance weighting).
        """
        if len(self.markers) < 1:
            return None

        # Вычисляем расстояния от объекта до всех меток в пикселях
        distances = []
        for (mu, mv, mx, my, mz) in self.markers:
            d = np.hypot(mu - u, mv - v)
            distances.append(d)

        # Берём k ближайших (или все, если меток меньше)
        k = min(k, len(self.markers))
        idx = np.argsort(distances)[:k]

        # Веса: обратные расстояния (добавляем epsilon)
        eps = 1e-6
        weights = [1.0 / (distances[i] + eps) for i in idx]
        wsum = sum(weights)

        # Интерполируем координаты
        x = sum(weights[j] * self.markers[idx[j]][2] for j in range(k)) / wsum
        y = sum(weights[j] * self.markers[idx[j]][3] for j in range(k)) / wsum
        z = sum(weights[j] * self.markers[idx[j]][4] for j in range(k)) / wsum

        return (x, y, z)

def main(args=None):
    rclpy.init(args=args)
    node = ObjectLocalization()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()