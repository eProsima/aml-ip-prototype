"""Setup file to create amlip_nodes library."""
from setuptools import setup

with open('README.md', 'r') as f:
    long_description = f.read()

package_name = 'amlip_nodes'

file_packages = [
    package_name,
    package_name + '/aml',
    package_name + '/dds',
    package_name + '/dds/entity',
    package_name + '/dds/network',
    package_name + '/dds/node',
    package_name + '/exception',
    package_name + '/tool',
]

setup(
    name=package_name,
    version='0.0.0',
    packages=file_packages,
    long_description=long_description,
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='eprosima',
    maintainer_email='javierparis@eprosima.com',
    description='Node package for AML-IP prototype',
    license='Apache License, Version 2.0',
    tests_require=['pytest'],
    test_suite='tests',
    entry_points={
        'console_scripts': [
            'amlip_main = amlip_nodes.tool.run_mainNode:main',
            'amlip_computing = amlip_nodes.tool.run_computingNode:main',
            'amlip_status = amlip_nodes.tool.run_statusNode:main',
        ],
    },
)
