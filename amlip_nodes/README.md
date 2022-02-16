
# AML-IP NODES

This directory contains a python colcon package with the implementation (in python) of the different nodes to
execute a prototype AML network.

## TODO

1. Change prints to logging module

## Comments about Ament Python for colcon

1. Library must have in root dir files
    1. Subdirectory `resource`
        1. Empty file called as the library
    1. File `setup.py`
        1. Add library empty file to data_files to avoid colcon warning
        1. Add `package.xml` file to data_files to avoid colcon warning
    1. File `setup.cfg`
    1. File `package.xml`
        1. Must include `<export> <build_type>ament_python</build_type> </export>` inside `<package>`
    1. Subdirectory called as the package with:
        1. `__init__.py` file
1. `__init__.py` file in every subdirectory with python sources
1. tests must be called `<module>_test.py` in order to be found
