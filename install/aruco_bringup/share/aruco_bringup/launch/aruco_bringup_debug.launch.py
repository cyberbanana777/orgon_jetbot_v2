# file: launch/combined_launch.py
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    return LaunchDescription([
        # Первый включаемый launch-файл (из пакета package_a)
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([
                PathJoinSubstitution([
                    FindPackageShare('aruco_analyzer'),  # имя пакета, где лежит launch-файл
                    'launch',
                    'debug.launch.py'
                ])
            ])
        ),

        ExecuteProcess(
            cmd=[
                'rqt',
                '--perspective-file',
                PathJoinSubstitution([
                    FindPackageShare('aruco_bringup'),
                    'rqt_config',
                    'debug.perspective'   # имя файла перспективы
                ])
            ],
            output='screen',
            name='rqt_aruco_view'
        ),
    ])