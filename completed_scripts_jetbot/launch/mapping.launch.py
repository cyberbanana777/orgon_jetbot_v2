# Copyright (c) 2025 Alice Zenina and Alexander Grachev RTU MIREA (Russia)
# SPDX-License-Identifier: MIT
# Details in the LICENSE file in the root of the package.

import os
from launch import LaunchDescription
from launch.actions import (
    OpaqueFunction,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def launch_setup(context, *args, **kwargs):

    config_dir = get_package_share_directory('completed_scripts_jetbot')
    path_to_config_1 = os.path.join(config_dir, 'config', 'mapper_params_real.yaml')


    mapping_launch = IncludeLaunchDescription(
            PythonLaunchDescriptionSource([
                PathJoinSubstitution([
                    FindPackageShare('slam_toolbox'),
                    'launch',
                    'online_sync_launch.py'
                ])
            ]),
            launch_arguments={
                'slam_params_file': path_to_config_1,
                'use_sim_time': 'false',
                'debug': 'true',
            }.items(),
        )
    
    return [mapping_launch]


def generate_launch_description():
    return LaunchDescription([
        OpaqueFunction(function=launch_setup)
    ])