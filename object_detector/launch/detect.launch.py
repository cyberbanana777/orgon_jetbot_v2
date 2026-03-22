from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Узел object_localization
        Node(
            package='object_detector',          # имя вашего пакета
            executable='coordinates_object', # имя entry point из setup.py
            name='coordinates_object',      # имя узла (можно переопределить)
            output='screen',                 # вывод логов в терминал
        ),
        # Узел 
        Node(
            package='object_detector',
            executable='object_detector',
            name='object_detector',
            output='screen',
            # Переопределение параметров (значения по умолчанию из кода)
            parameters=[{
                'min_area': 500,
                'debug_window': True,
                'display_scale': 0.5,
                'canny_low': 30,
                'canny_high': 100,
                'circularity_thresh': 0.8,
            }]
        ),
        Node(
            package='object_detector',          # имя вашего пакета
            executable='start_detector', # имя entry point из setup.py
            name='start_detector',      # имя узла (можно переопределить)
            output='screen',                 # вывод логов в терминал
        )
    ])