
# AML-IP NODES

This directory contains a python colcon package with the implementation (in python) of the different nodes to
execute a prototype AML network for workload distributed scenario.

## Library Modules

- **aml**: This module contains the files that implement the proper AML-IP nodes for AML, those are:
  - AML-IP Status Node
  - AML-IP Main Node
  - AML-IP Computing Node

- **dds**: This module contains the files that implement classes needed to create AML nodes using DDS entities:
  - *entity*: Implementation of the DDS entities using fastdds-python library
  - *network*: Macros to configure the DDS communication
  - *node*: Implementation of the AML nodes at DDS level

- **exception**: This module contains the different exceptions used along the project.

- **log_module**: This module contains the global variable to log along the project.

- **tool**: This module contains the scripts to run each kind of the nodes by different executables:
  - `./install/amlip_nodes/lib/amlip_nodes/amlip_status`
  - `./install/amlip_nodes/lib/amlip_nodes/amlip_main`
  - `./install/amlip_nodes/lib/amlip_nodes/amlip_computing`

## Comments about Ament Python for colcon

1. Library must have in root dir files
    1. Subdirectory `resource`
        1. Empty file called as the library
    1. File `setup.py`
        1. Add library empty file to data_files to avoid colcon warning
        1. Add `package.xml` file to data_files to avoid colcon warning
        1. Add any of the subdirectories of the library that contains python code
    1. File `setup.cfg`
    1. File `package.xml`
        1. Must include `<export> <build_type>ament_python</build_type> </export>` inside `<package>`
    1. Subdirectory called as the package with:
        1. `__init__.py` file
1. `__init__.py` file in every subdirectory with python sources
1. tests must be called `*_test.py` in order to be found
    1. Each internal test must be encapsulated in a function with name `test_*`
