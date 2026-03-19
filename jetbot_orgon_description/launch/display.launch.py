import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    pkg_share = get_package_share_directory('jetbot_orgon_description')

    xacro_file = os.path.join(pkg_share, 'urdf', 'jetbot.urdf.xacro')
    rviz_config_file = os.path.join(pkg_share, 'rviz', 'jetbot.rviz')

    # Xacro → URDF
    robot_description = ParameterValue(
        Command(['xacro ', xacro_file]),
        value_type=str
    )

    robot_description_param = {
        'robot_description': robot_description
    }

    # robot_state_publisher
    rsp_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[robot_description_param],
        output='screen'
    )

    # joint_state_publisher_gui (для движения суставов)
    jsp_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        output='screen'
    )

    # RViz с конфигом
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', rviz_config_file],
        output='screen'
    )

    return LaunchDescription([
        rsp_node,
        jsp_node,
        rviz_node
    ])