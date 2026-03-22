# Copyright (c) 2025 Alice Zenina and Alexander Grachev RTU MIREA (Russia)
# SPDX-License-Identifier: MIT
# Details in the LICENSE file in the root of the package.

import os
from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    OpaqueFunction,
    TimerAction
)
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch.conditions import IfCondition

def launch_setup(context, *args, **kwargs):
    path_to_pkg = get_package_share_directory('completed_scripts_jetbot')

    acml_config_yaml_real = os.path.join(path_to_pkg, 'config', 'amcl_params_real.yaml')
    map_file = os.path.join(path_to_pkg, 'maps', 'poligon.yaml')
        # map_file = os.path.join(path_to_pkg, 'maps', 'G210_with_boxes_map.yaml')

    acml_node_real = Node(
        package='nav2_amcl',
        executable='amcl',
        name='amcl',
        output='screen',
        parameters=[acml_config_yaml_real],
    )


    lifecycle_manager = Node(
        package='nav2_lifecycle_manager',
        executable='lifecycle_manager',
        name='lifecycle_manager',
        output='screen',
        parameters=[
            {'autostart': True},
            {
                'node_names': [
                    'map_server',
                    'amcl',
                    ]
            }
        ]
    )
        
    map_loader_real = TimerAction(
        period=2.0,  # Задержка в секундах
        actions=[
            Node(
                package='nav2_map_server',
                executable='map_server',
                name='map_server',
                output='screen',
                parameters=[{'use_sim_time': False},
                            {'yaml_filename': map_file}],
            ),
        ]
    )

    return [
        acml_node_real,
        lifecycle_manager,
        map_loader_real,
    ]

def generate_launch_description():
    return LaunchDescription([
        OpaqueFunction(function=launch_setup)
    ])
