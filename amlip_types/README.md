
# AML-IP TYPES

This directory contains a cmake colcon package with the types used in the library AML-IP for FastDDS python.

## Use library

In order to use this library form a different module, source the installation module
and import the library `amlip_types`.

## Create types files

It requires to use the FastDDSGen tool.

```bash
# From colcon workspace

# Convert idl files to sources
./<path-to-fastddsgen-installation>/scripts/fastddsgen -python -cs -d src/amlip/amlip_types/sources/py/ src/amlip/amlip_types/idl/* -replace
# ./src/fastddsgen/scripts/fastddsgen -python -cs -d src/amlip/amlip_types/sources/py/ src/amlip/amlip_types/idl/* -replace
```
