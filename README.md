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
# Executables that could be run:
# AML_IP_DL | AML_IP_Engine | AML_IP_DiscoveryServer
./${WORKSPACE_PATH}/install/AML_IP_Prototype/bin/AML_IP_DL # params (use -h to check all arguments)
```

## Requirements

fastrtps installation

## Internal

TODO: this is an internal informative part for version controlling. Eliminate before public repository

### Design decisions

#### Executables

The project have two executables:
Both executables will count with a Fast-DDS participant and a mock that creates random data along the execution.

1. engine_participant
    1. It has a DataWriter in topic `_aml_ip_topic_dloutput` with type `AML_IP_Relation`
        1. It generates random data with a frecuency of `f`
    1. executable:
        1. path: `${WORKSPACE_PATH}/install/amlip`
        1. name: `AML_IP_Engine`

1. dl_participant
    1. It has a DataReader in topic `_aml_ip_topic_dloutput` with type `AML_IP_Relation`
        1. The information received will be shown in screen
    1. It has a DataWriter in topic `_aml_ip_topic_atomization` with type `AML_IP_Atom`
        1. It generates random data with a frecuency of `f`
    1. It has a DataReader in topic `_aml_ip_topic_atomization` with type `AML_IP_Atom`
        1. The information received will be shown in screen
    1. executable:
        1. path: `${WORKSPACE_PATH}/install/amlip`
        1. name: `AML_IP_DL`

#### Namespaces

No namespaces are used.

#### Thirdparty

One header only file is needed:

1. `optionparser.h`
    1. Library to parser arguments in main
    1. Included in *src/cpp/utils* directory
