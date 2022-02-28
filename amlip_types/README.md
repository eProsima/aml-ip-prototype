
# AML-IP TYPES

This directory contains a cmake colcon package with the types used in the library AML-IP for FastDDS python.

## Use library

In order to use this library form a different python module, source the installation module
and import any of the libraries:

- `status`
- `multiservice`
- `job`
- `inference`

## Create types files

It requires to use the FastDDSGen tool.

```bash
# Convert idl files to sources

# From colcon workspace
./<path-to-fastddsgen-installation>/scripts/fastddsgen -python -cs -d src/amlip/amlip_types/sources/py/ src/amlip/amlip_types/idl/* -replace

# From colcon workspace with fastddsgen downloaded and installed:
# ./src/fastddsgen/scripts/fastddsgen -python -cs -d src/amlip/amlip_types/sources/py/ src/amlip/amlip_types/idl/* -replace
```
