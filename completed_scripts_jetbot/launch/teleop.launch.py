# Copyright (c) 2026 Alice Zenina and Alexander Grachev RTU MIREA (Russia)
# SPDX-License-Identifier: MIT
# Details in the LICENSE file in the root of the package.

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration, PythonExpression
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # Аргумент use_sim_time (по умолчанию false)
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time',
        default_value='false',
        description='Use simulation time',
        choices=['true', 'false']
    )

    # Путь к YAML-файлу с параметрами.
    # Предполагаем, что файл лежит в папке config пакета completed_scripts_jetbot.
    params_file = os.path.join(
        get_package_share_directory('completed_scripts_jetbot'),
        'config',
        'teleop_params.yaml'
    )

    # Формируем строку "use_sim_time:=<значение>" через PythonExpression
    use_sim_time_param = PythonExpression([
        '"use_sim_time:=" + "', LaunchConfiguration('use_sim_time'), '"'
    ])
    # Пояснение: PythonExpression склеит части в одно выражение и вычислит его.
    # Результат будет строкой вида "use_sim_time:=true".

    # Запуск teleop в отдельном терминале
    teleop_cmd = ExecuteProcess(
        cmd=[
            'gnome-terminal', '--',  # если gnome-terminal не установлен, замени на xterm -e и т.п.
            'ros2', 'run', 'teleop_twist_keyboard', 'teleop_twist_keyboard',
            '--ros-args',
            '--params-file', params_file,
            '-p', use_sim_time_param
        ],
        output='screen'
    )

    return LaunchDescription([
        use_sim_time_arg,
        teleop_cmd
    ])