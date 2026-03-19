import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Pose2D
import tkinter as tk
from tkinter import ttk
import threading

class ScaraTeleopGUI(Node):
    def __init__(self):
        super().__init__('scara_teleop_gui')
        
        # Параметры пределов
        self.declare_parameter('x_min', -0.2)
        self.declare_parameter('x_max', 0.2)
        self.declare_parameter('y_min', -0.2)
        self.declare_parameter('y_max', 0.2)
        self.declare_parameter('grip_min', 0.0)
        self.declare_parameter('grip_max', 0.09)
        
        self.x_min = self.get_parameter('x_min').value
        self.x_max = self.get_parameter('x_max').value
        self.y_min = self.get_parameter('y_min').value
        self.y_max = self.get_parameter('y_max').value
        self.grip_min = self.get_parameter('grip_min').value
        self.grip_max = self.get_parameter('grip_max').value
        
        # Издатель целевой точки
        self.pose_pub = self.create_publisher(Pose2D, 'hand_of_god/target_pose', 10)
        
        # Текущие значения
        self.x_val = self.x_max
        self.y_val = 0.0
        self.grip_val = self.grip_min
        
        # Запуск GUI в отдельном потоке
        self.gui_thread = threading.Thread(target=self.run_gui)
        self.gui_thread.daemon = True
        self.gui_thread.start()
        
        self.get_logger().info('Scara Teleop GUI started')
    
    def run_gui(self):
        self.root = tk.Tk()
        self.root.title("SCARA Teleoperation")
        
        # X слайдер
        ttk.Label(self.root, text="X (м)").grid(row=0, column=0, padx=5, pady=5)
        self.x_scale = ttk.Scale(self.root, from_=self.x_min, to=self.x_max, 
                                  orient=tk.HORIZONTAL, command=self.on_x_change)
        self.x_scale.set(self.x_val)
        self.x_scale.grid(row=0, column=1, padx=5, pady=5, sticky='we')
        self.x_label = ttk.Label(self.root, text=f"{self.x_val:.3f}")
        self.x_label.grid(row=0, column=2, padx=5, pady=5)
        
        # Y слайдер
        ttk.Label(self.root, text="Y (м)").grid(row=1, column=0, padx=5, pady=5)
        self.y_scale = ttk.Scale(self.root, from_=self.y_min, to=self.y_max,
                                  orient=tk.HORIZONTAL, command=self.on_y_change)
        self.y_scale.set(self.y_val)
        self.y_scale.grid(row=1, column=1, padx=5, pady=5, sticky='we')
        self.y_label = ttk.Label(self.root, text=f"{self.y_val:.3f}")
        self.y_label.grid(row=1, column=2, padx=5, pady=5)
        
        # Grip слайдер (сила схвата)
        ttk.Label(self.root, text="Grip").grid(row=2, column=0, padx=5, pady=5)
        self.grip_scale = ttk.Scale(self.root, from_=self.grip_min, to=self.grip_max,
                                     orient=tk.HORIZONTAL, command=self.on_grip_change)
        self.grip_scale.set(self.grip_val)
        self.grip_scale.grid(row=2, column=1, padx=5, pady=5, sticky='we')
        self.grip_label = ttk.Label(self.root, text=f"{self.grip_val:.1f}")
        self.grip_label.grid(row=2, column=2, padx=5, pady=5)
        
        # Кнопка отправки (опционально) или можно отправлять постоянно при изменении
        self.send_button = ttk.Button(self.root, text="Отправить", command=self.publish_pose)
        self.send_button.grid(row=3, column=0, columnspan=3, pady=10)
        
        # Настройка растягивания колонок
        self.root.columnconfigure(1, weight=1)
        
        self.root.mainloop()
    
    def on_x_change(self, value):
        self.x_val = float(value)
        self.x_label.config(text=f"{self.x_val:.3f}")
        
    
    def on_y_change(self, value):
        self.y_val = float(value)
        self.y_label.config(text=f"{self.y_val:.3f}")
        
    
    def on_grip_change(self, value):
        self.grip_val = float(value)
        self.grip_label.config(text=f"{self.grip_val:.3f}")
        
    
    def publish_pose(self):
        msg = Pose2D()
        msg.x = self.x_val
        msg.y = self.y_val
        msg.theta = self.grip_val
        self.pose_pub.publish(msg)
        self.get_logger().debug(f'Published: x={msg.x:.3f}, y={msg.y:.3f}, grip={msg.theta:.3f}')

def main(args=None):
    rclpy.init(args=args)
    node = ScaraTeleopGUI()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        if hasattr(node, 'root') and node.root:
            node.root.quit()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()