#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import CompressedImage
from std_msgs.msg import String
import cv2
import numpy as np

class CombinedDetector(Node):
    def __init__(self):
        super().__init__('combined_detector')

        # ----- Параметры -----
        self.declare_parameter('min_area', 500)
        self.declare_parameter('debug_window', True)
        self.declare_parameter('display_scale', 0.5)
        self.declare_parameter('canny_low', 30)
        self.declare_parameter('canny_high', 100)
        self.declare_parameter('circularity_thresh', 0.8)

        # Цветовые диапазоны (HSV)
        self.color_ranges = {
        'red':    [((0, 50, 50),   (10, 255, 255)),((170, 50, 50), (180, 255, 255))],
        'green':  [((40, 50, 50),  (80, 255, 255))],
        'cyan':   [((80, 80, 80),  (100, 255, 255))]
}

        # Получаем значения параметров
        self.min_area = self.get_parameter('min_area').get_parameter_value().integer_value
        self.debug_window = self.get_parameter('debug_window').get_parameter_value().bool_value
        self.display_scale = self.get_parameter('display_scale').get_parameter_value().double_value
        self.canny_low = self.get_parameter('canny_low').get_parameter_value().integer_value
        self.canny_high = self.get_parameter('canny_high').get_parameter_value().integer_value
        self.circularity_thresh = self.get_parameter('circularity_thresh').get_parameter_value().double_value

        # Подписка на сжатое изображение
        self.subscription = self.create_subscription(
            CompressedImage,
            '/camera/camera/color/image_raw/compressed',
            self.image_callback,
            10
        )

        # Публикация результатов
        self.publisher = self.create_publisher(String, '/detected_objects', 10)

        if self.debug_window:
            cv2.namedWindow("Combined Detector", cv2.WINDOW_NORMAL)

        self.get_logger().info("Combined detector started, waiting for images...")

    def image_callback(self, msg: CompressedImage):
        # Декодируем сжатое изображение
        try:
            np_arr = np.frombuffer(msg.data, np.uint8)
            frame = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
            if frame is None:
                self.get_logger().warn("Failed to decode image")
                return
        except Exception as e:
            self.get_logger().error(f"Image decoding error: {e}")
            return

        # Обрабатываем кадр
        result_frame, detections = self.process_frame(frame)

        # Публикуем и логируем результаты
        for color, shape, cx, cy in detections:
            msg_str = f"{shape}, {color}, {cx}, {cy}"
            self.publisher.publish(String(data=msg_str))
            self.get_logger().info(msg_str)

        # Отображаем результат
        if self.debug_window:
            if self.display_scale != 1.0:
                h, w = result_frame.shape[:2]
                new_w = int(w * self.display_scale)
                new_h = int(h * self.display_scale)
                result_frame = cv2.resize(result_frame, (new_w, new_h))
            cv2.imshow("Combined Detector", result_frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                rclpy.shutdown()

    def process_frame(self, frame):
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        result_frame = frame.copy()
        detections = []

        for color_name, ranges in self.color_ranges.items():
            mask_total = None
            for (lower, upper) in ranges:
                lower = np.array(lower, dtype=np.uint8)
                upper = np.array(upper, dtype=np.uint8)
                mask = cv2.inRange(hsv, lower, upper)
                if mask_total is None:
                    mask_total = mask
                else:
                    mask_total = cv2.bitwise_or(mask_total, mask)
                    mask = mask_total

            kernel = np.ones((5, 5), np.uint8)
            mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
            mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            for cnt in contours:
                area = cv2.contourArea(cnt)
                if area < self.min_area:
                    continue

                M = cv2.moments(cnt)
                if M["m00"] == 0:
                    continue
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])

                shape = self.classify_shape(cnt)
                detections.append((color_name, shape, cx, cy))

                cv2.drawContours(result_frame, [cnt], -1, (0, 255, 0), 2)
                cv2.circle(result_frame, (cx, cy), 5, (0, 0, 255), -1)
                label = f"{shape} {color_name}"
                cv2.putText(result_frame, label, (cx + 10, cy - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)

        return result_frame, detections

    def classify_shape(self, contour):
        peri = cv2.arcLength(contour, True)
        if peri == 0:
            return "unknown"

        approx = cv2.approxPolyDP(contour, 0.04 * peri, True)
        vertices = len(approx)

        if vertices == 4:
            return "cube"

        area = cv2.contourArea(contour)
        if area > 0:
            circularity = 4 * np.pi * area / (peri * peri)
            if circularity > self.circularity_thresh:
                return "cylinder"
            else:
                return "duck"
        return "unknown"

def main(args=None):
    rclpy.init(args=args)
    node = CombinedDetector()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    main()