from setuptools import find_packages, setup

package_name = 'manipulator_control'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', ['launch/manipulator.launch.py']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='kesh',
    maintainer_email='kesh@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'scara_teleop_gui = manipulator_control.scara_teleop_gui:main',
            'scara_ik_planner = manipulator_control.scara_ik_planner:main',
        ],
    },
)
