# aml-ip-prototype

AML Integrating Platform is a network architecture that connects different AML Nodes in order to create
a distributed independent AML network along the internet.
The following repository store the first prototype that represents a MockUp of AML Nodes and could be use
as an example of the communication process between nodes using Fast DDS communciation.
This is an eProsima repository.

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
# Executables that could be run:
# AML_IP_DL | AML_IP_Engine | AML_IP_DiscoveryServer
cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL # params (use -h to check all arguments)
```

### Execute a simple example in WAN

Example for the scenario with one AML-DL node and one AML-Engine, connected by a Discovery Server.
For this scenario, it is required to open a port in the router for each node that will be executed under the router net.
If all nodes are in the same LAN, this is not required.

First of all a Discovery Server Participant must be deployed in order to manage the discovery phase.
This server will not close until enter is pressed in that terminal (do not execute in background mode with --time=0).
This Server run in a machine with IP `192.168.1.2` and the router asociated, with public IP `1.1.1.1` have
the port `5101` forwarded to `192.168.1.2:5101`.

```sh
# Params available for AML_IP_DiscoveryServer
# -h            --help               Produce help message.
# -t <num>      --tcp-port=<num>     Port to listen as TCP server (Default 5100).
# -a <address>  --address=<address>  Public IP address to connect from outside the LAN [Required].
# --time                             Time in seconds until the server closes, if 0 wait for user input (Default 0).

cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer --tcp-port=5101 --address="1.1.1.1"
```

Secondly, execute an AML-DL Participant that will produce 50 messages, with a period of 3 seconds, of max size of 15 relations.
This DL runs in a host with IP `192.168.1.3` under a router with public IP `2.2.2.2` with port `5201` forwarded to `192.168.1.3:5201`.

```sh
# Params available for AML_IP_DL
# -h  --help                     Produce help message.
# -p <float>  --period=<float>   Period to send new random data (Default: 2).
# -s <num>  --samples=<num>      Number of samples to send (Default: 10). With samples=0 it keept sending till enter is pressed
# -l <num>  --size=<num>         Max number of relations in data to send(Default: 5). This value also works as seed for randome generation.
# --connection-port=<num>        Port where the TCP server is listening (Default: 5100).
# --connection-address=<address> IP address where the TCP server is listening (Default: '').
# --listening-port=<num>         Port to listen as TCP server (Default: -1).
# --listening-address=<address>  IP address to listen as TCP server (Default: '').

cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --period=3 --samples=50 --size=15 --connection-port=5101 --connection-address="1.1.1.1" --listening-port=5201 --listening-address="2.2.2.2"
```

Finally, while the Discovery Server and the DL Participant are running, we execute an Engine Participant.
This Engine will share its atomization each 10 seconds, with a maximum size of 50 atoms and sending 10 samples.
This Engine runs in a host with IP `192.168.1.4` under a router with public IP `3.3.3.3` with port `5301` forwarded to `192.168.1.4:5301`.

```sh
# Params available for AML_IP_Engine
# -h  --help                     Produce help message.
# -p <float>  --period=<float>   Period to send new random data (Default: 2).
# -s <num>  --samples=<num>      Number of samples to send (Default: 10). With samples=0 it keept sending till enter is pressed
# -l <num>  --size=<num>         Max number of relations in data to send(Default: 5). This value also works as seed for randome generation.
# --connection-port=<num>        Port where the TCP server is listening (Default: 5100).
# --connection-address=<address> IP address where the TCP server is listening (Default: '').
# --listening-port=<num>         Port to listen as TCP server (Default: -1).
# --listening-address=<address>  IP address to listen as TCP server (Default: '').

cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --period=10 --samples=10 --size=50 --connection-port=5101 --connection-address="1.1.1.1" --listening-port=5301 --listening-address="3.3.3.3"
```

The behaviour expected is the Engine starts to listen the DL messages, and print them in `stdout`.
Could be checked that the communication is correct by comparing the DL sent messages and the Engine received ones.

In order to create bigger examples, just add new hosts with new nodes of DL or Engine type and check that the Engines listen to every DL node and share the atomization information between them.

**WARNING**: if this experiment is run with some or all the nodes in same local host, the router must allow loopback functionallity.
And port mapping cannot be shared between nodes.

### Execute a simple example in one host

These commands executes the same architecture than the example above, but in the case all the nodes run in the same host.

Discovery Server Participan

```sh
cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer --tcp-port=5101 --address="127.0.0.1"
```

DL Participant

```sh
cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --period=3 --samples=50 --size=15 --connection-port=5101 --connection-address="127.0.0.1" --listening-port=5201 --listening-address="127.0.0.1"
```

Engine Participant

```sh
cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --period=10 --samples=10 --size=50 --connection-port=5101 --connection-address="127.0.0.1" --listening-port=5301 --listening-address="127.0.0.1"
```

## Requirements

Fast DDS installation.
<https://github.com/eProsima/Fast-DDS>

## Internal

TODO: this is an internal informative part for version controlling. Eliminate before public repository

### Design decisions

#### Executables

The project have two executables:
Both executables will count with a Fast-DDS participant and a mock that creates random data along the execution.

1. engine_participant
    1. It has a DataWriter in topic `_aml_ip_topic_dloutput` with type `AML_IP_DLOutput`
        1. It generates random data with a frecuency of `f`
    1. executable:
        1. path: `${WORKSPACE_PATH}/install/amlip`
        1. name: `AML_IP_Engine`

1. dl_participant
    1. It has a DataReader in topic `_aml_ip_topic_dloutput` with type `AML_IP_DLOutput`
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
