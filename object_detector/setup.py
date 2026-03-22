from setuptools import find_packages, setup
import os
from glob import glob
package_name = 'object_detector'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (
            os.path.join("share", package_name, "launch"),
            glob("launch/*.launch.py"),
        ),
        (
            os.path.join("share", package_name, "rqt_config"),
            glob("rqt_config/*.perspective"),
        ),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='banana-killer',
    maintainer_email='sashagrachev2005@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'coordinates_object = object_detector.coordinates_object:main',
            'object_detector = object_detector.object_detector:main',
            'start_detector = object_detector.start_detector:main',
        ],
    },
)
