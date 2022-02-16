"""Setup file to create amlip_nodes library."""
from setuptools import setup

with open('README.md', 'r') as f:
    long_description = f.read()

package_name = 'amlip_nodes'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
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
)
