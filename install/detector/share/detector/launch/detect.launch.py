from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # Аргументы для object_detector
    min_area_arg = DeclareLaunchArgument(
        'min_area', default_value='500',
        description='Minimum contour area for detection'
    )
    debug_window_arg = DeclareLaunchArgument(
        'debug_window', default_value='true',
        description='Show debug window'
    )
    display_scale_arg = DeclareLaunchArgument(
        'display_scale', default_value='0.5',
        description='Scale for display window'
    )
    canny_low_arg = DeclareLaunchArgument(
        'canny_low', default_value='30',
        description='Canny low threshold'
    )
    canny_high_arg = DeclareLaunchArgument(
        'canny_high', default_value='100',
        description='Canny high threshold'
    )
    circularity_thresh_arg = DeclareLaunchArgument(
        'circularity_thresh', default_value='0.8',
        description='Circularity threshold for cylinder detection'
    )

    # Аргументы для object_localizer
    grid_origin_x_arg = DeclareLaunchArgument(
        'grid_origin_x', default_value='0.0',
        description='Origin X of aruco grid'
    )
    grid_origin_y_arg = DeclareLaunchArgument(
        'grid_origin_y', default_value='0.0',
        description='Origin Y of aruco grid'
    )
    cell_size_arg = DeclareLaunchArgument(
        'cell_size', default_value='0.1',
        description='Cell size of aruco grid (meters)'
    )
    grid_width_arg = DeclareLaunchArgument(
        'grid_width', default_value='5',
        description='Number of aruco markers in X direction'
    )
    grid_height_arg = DeclareLaunchArgument(
        'grid_height', default_value='5',
        description='Number of aruco markers in Y direction'
    )

    # Узел детектора объектов
    detector_node = Node(
        package='detector',        # замените на имя вашего пакета
        executable='shape_detector',       # имя исполняемого файла из setup.py
        name='shape_detector',
        output='screen',
        parameters=[{
            'min_area': LaunchConfiguration('min_area'),
            'debug_window': LaunchConfiguration('debug_window'),
            'display_scale': LaunchConfiguration('display_scale'),
            'canny_low': LaunchConfiguration('canny_low'),
            'canny_high': LaunchConfiguration('canny_high'),
            'circularity_thresh': LaunchConfiguration('circularity_thresh'),
        }]
    )

    # Узел локализации объектов
    localizer_node = Node(
        package='detector',        # замените на имя вашего пакета
        executable='cords_detector',      # имя исполняемого файла из setup.py
        name='cords_detector',
        output='screen',
        parameters=[{
            'grid_origin_x': LaunchConfiguration('grid_origin_x'),
            'grid_origin_y': LaunchConfiguration('grid_origin_y'),
            'cell_size': LaunchConfiguration('cell_size'),
            'grid_width': LaunchConfiguration('grid_width'),
            'grid_height': LaunchConfiguration('grid_height'),
        }]
    )

    return LaunchDescription([
        min_area_arg,
        debug_window_arg,
        display_scale_arg,
        canny_low_arg,
        canny_high_arg,
        circularity_thresh_arg,
        grid_origin_x_arg,
        grid_origin_y_arg,
        cell_size_arg,
        grid_width_arg,
        grid_height_arg,
        detector_node,
        localizer_node,
    ])