from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='manipulator_control',
            executable='scara_teleop_gui',
            name='scara_teleop_gui',
            output='screen'
        ),
        Node(
            package='manipulator_control',
            executable='scara_ik_planner',
            name='scara_ik_planner',
            parameters=[{'a1': 0.054, 'a2': 0.100, 'ARM': -1}],
            output='screen'
        ),
    ])