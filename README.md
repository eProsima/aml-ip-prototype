# aml-ip-prototype

## Instructions

Set `${WORKSPACE_PATH}` environmental variable for the path AML-IP workspace.

Download the workspace:

```sh
mkdir -p ${WORKSPACE_PATH}
cd ${WORKSPACE_PATH}
wget https://raw.githubusercontent.com/eProsima/aml-ip-prototype/main/amlip.repos
mkdir src
vcs import src < amlip.repos
```

Build the workspace:

```sh
cd ${WORKSPACE_PATH}
colcon build --cmake-clean-cache
```

Execute in a terminal:

```sh
cd ${WORKSPACE_PATH}
# Scripts that could be run:
# AML.sh | dl.sh | engine.sh | AML_TCP.sh
bash ${WORKSPACE_PATH}/src/amlip/scripts/AML.sh ${WORKSPACE_PATH}/AML-IP
```

## Requirements

fastrtps installation

## Internal

TODO: this is an internal informative part for version controlling

### Design decisions

#### Executables
The project have two executables:
Both executables will count with a Fast-DDS participant and a mock that creates random data along the execution.

1. engine_participant
    1. It has a DataWriter in topic `_aml_ip_topic_dloutput` with type `AML_IP_Relation`
        1. It generates random data with a frecuency of `f`

1. dl_participant
    1. It has a DataReader in topic `_aml_ip_topic_dloutput` with type `AML_IP_Relation`
        1. The information received will be shown in screen
    1. It has a DataWriter in topic `_aml_ip_topic_atomization` with type `AML_IP_Atom`
        1. It generates random data with a frecuency of `f`
    1. It has a DataReader in topic `_aml_ip_topic_atomization` with type `AML_IP_Atom`
        1. The information received will be shown in screen

#### Namespaces

No namespaces are used.

#### Thirdparty

One header only file is needed:

1. `optionparser.h`
    1. Library to parser arguments in main
    1. Included in *src/cpp/utils* directory

### Built steps

1. Design of IDL files for data types
1. Build Fast project and build fastddsgen
    1. ws$> colcon build
    1. ws/src/fastrtpsgen$> gradle assemble
1. Execute fastddsgen with idl Files
    1. ws$> fastrtpsgen/scripts/fastddsgen src/types/Atomization/Atomization.idl

### Tests

1. Transport by default
    1. cmd: `bash ./src/amlip/scripts/AML.sh`
    1. Working in same host

1. TCP transport in same bash
    1. cmd: `bash ./src/amlip/scripts/AML.sh`
    1. Working in same host

1. TCP transport in different bash
    1. Working in same host
        1. cmd:
            1. `bash ./src/amlip/scripts/dl.sh . 5100 "127.0.0.1"`
            1. `bash ./src/amlip/scripts/engine.sh . 5100 "127.0.0.1"`