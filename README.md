# AML Integrating Platform - Prototype

AML Integrating Platform is a network architecture that connects different AML Nodes in order to create
a distributed independent AML network along the internet.
The following repository store the first prototype that represents a MockUp of AML Nodes and could be use
as an example of the communication process between nodes using Fast DDS communciation.

## Requirements

> :warning: cppupnp requires `Boost 1.71`.
## Installation Instructions

Set `${WORKSPACE_PATH}` environmental variable for the path AML-IP workspace.

Download the workspace:

```sh
mkdir -p ${WORKSPACE_PATH}/src
cd ${WORKSPACE_PATH}
wget https://raw.githubusercontent.com/eProsima/aml-ip-prototype/main/amlip.repos
vcs import src < amlip.repos
```

Build the workspace:

```sh
cd ${WORKSPACE_PATH}
colcon build
```

## Executables

The installation of this prototype create an executable for every kind of node.
These executables are divided in two: *Discovery Server (DS)* and *TCP*.
This division remains on the discovery protocol used to know new entities.
It is recommended to use the *DS* nodes, as it brings advantages and it has no limitations over *TCP* version.

1. Using *DS* the nodes connected with the *DS node* will automatically know each other. Only the *DS Node* address must be known beforehand.
1. Using *TCP* the discovery will be static, so the address of each node must be known beforehand, and only two nodes could be connected.

These are the executables the procuct installs:

1. Discovery by *Discovery Server*
    1. AML_IP_DiscoveryServer
        1. Execute an AML Discovery Server Node that centralizes the new nodes discovery.
    1. AML_IP_DL
        1. Execute an AML DL Node that publish in `DL` topic.
    1. AML_IP_Engine
        1. Execute an AML Engine Node that publish in `Atom` topic and subscribes in `DL` and `Atom` topics.

1. Discovery by *Static Discovery*
    1. AML_IP_DL_TCP
        1. Execute an AML DL Node that publish in `DL` topic.
    1. AML_IP_Engine_TCP
        1. Execute an AML Engine Node that publish in `Atom` topic and subscribes in `DL` and `Atom` topics.

Each executable has different input arguments that allows to parametrized the execution.
Some of these arguments are required when working in LAN or WAN to set addresses, and some others are just needed to change the time or measures in the exeution.

### Run one node in a shell

```sh
# Executables:
# AML_IP_DL | AML_IP_Engine | AML_IP_DiscoveryServer | AML_IP_DL_TCP | AML_IP_Engine_TCP
cd ${WORKSPACE_PATH}
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL # params (use -h to check arguments)
```

## Executing Examples

It is assummed that the user have a network architecture similar to the one described below to execute the examples correctly:

1. 3 different and independent NATs (under different routers or wifi connections).
1. 4 different machines, 2 under the first NAT and 2 in the other two NATs.
    1. It could be use only 3 hosts, if one of them is available to change from one NAT to the other for the *DS in LAN* example.

### Architecture

1. `Router-1`
    1. Public IP: `ROUTER_1_IP`
    1. Port forwarding: `ROUTER_1_IP:PORT_1 -> HOST_1_IP:PORT_1`
1. `Host-1`
    1. Under NAT from `Router-1`
    1. Internal IP: `HOST_1_IP`
1. `Host-1-2`
    1. Under NAT from `Router-1`
    1. Internal IP: `HOST_1_2_IP`
1. `Router-2`
    1. Public IP: `ROUTER_2_IP`
    1. Port forwarding: `ROUTER_2_IP:PORT_2 -> HOST_2_IP:PORT_2`
1. `Host-2`
    1. Under NAT from `Router-2`
    1. Internal IP: `HOST_2_IP`
1. `Router-3`
    1. Public IP: `ROUTER_3_IP`
    1. Port forwarding: `ROUTER_3_IP:PORT_3 -> HOST_3_IP:PORT_3`
1. `Host-3`
    1. Under NAT from `Router-3`
    1. Internal IP: `HOST_3_IP`

In each execution example, each of the commands must be executed in different shells, and in the host and NAT determined.

### DS in localhost

The communication with the *DS* will work by the `localhost (127.0.0.1:5100)` default address.
The nodes will communicate to each other by `Interprocess` transport.

```sh
# Discovery Server Node
# Host-1
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer --time 15 # time argument allows the node to drop after 15 seconds
```

```sh
# DL Node
# Host-1
source install/setup.bash
./install/amlip/bin/AML_IP_DL
```

```sh
# Engine Node
# Host-1
source install/setup.bash
./install/amlip/bin/AML_IP_Engine
```

### DS in LAN

The communication with the *DS* will work by setting the internal address:port where the *DS* is listening.
The nodes will communicate to each other by `UDP` default transport.

```sh
# Discovery Server Node
# Host-1
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer --time 15 --address=${ROUTER_1_IP} --port=${PORT_1}
```

```sh
# DL Node
# Host-1-2
source install/setup.bash
./install/amlip/bin/AML_IP_DL --connection-address=${ROUTER_1_IP} --connection-port=${PORT_1}
```

```sh
# Engine Node
# Host-1-2
source install/setup.bash
./install/amlip/bin/AML_IP_Engine --connection-address=${ROUTER_1_IP} --connection-port=${PORT_1}
```

With this same architecture, more *DL* and *Engine* nodes could be run at the same time, and they will communicate with every other node running at the time.

### DS in WAN

The communication with the *DS* will work by setting the external (router) address:port where the *DS* is listening.
The nodes will communicate to each other by `TCP`, and one of them must have portforwarding in its router transport.

```sh
# Discovery Server Node
# Host-1
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer --time 15 --address=${ROUTER_1_IP} --port=${PORT_1}
```

```sh
# DL Node
# Host-2
source install/setup.bash
./install/amlip/bin/AML_IP_DL --connection-address=${ROUTER_1_IP} --connection-port=${PORT_1} --listening-address=${ROUTER_2_IP} --listening-port=${PORT_2}
```

```sh
# Engine Node
# Host-3
source install/setup.bash
./install/amlip/bin/AML_IP_Engine --connection-address=${ROUTER_1_IP} --connection-port=${PORT_1} --listening-address=${ROUTER_3_IP} --listening-port=${PORT_3}
```

### Multiple DS in WAN

In this example there are multiple discovery servers that will interconnect.
One host will work as middle well known address, and the other two hosts will start a Discovery Server in each host that connects with the first one
One host will execute a DL node and the other an Engine node.
Ports `${PORT_2_x}` and `${PORT_3_x}` are any free port in host 2 and 3 respectively.

```sh
# Central Discovery Server Node
# Host-1
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer --listening-address=${ROUTER_1_IP} --listening-port=${PORT_1} --listening-id=1 --time 60
```

```sh
# Discovery Server Node
# Host-2
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer --listening-address="127.0.0.1"  --listening-port=${PORT_2_x} --listening-id=2 --connection-address=${ROUTER_1_IP} --connection-port=${PORT_1} --connection-id=1 --time 60
```

```sh
# DL Node
# Host-2
source install/setup.bash
./install/amlip/bin/AML_IP_DL --connection-address="127.0.0.1" --connection-port=${PORT_2_x} --listening-address=${ROUTER_2_IP} --listening-port=${PORT_2} -l 50 -s 6 -p 5 --id=2
```

```sh
# Discovery Server Node
# Host-3
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer --listening-address="127.0.0.1"  --listening-port=${PORT_3_x} --listening-id=3 --connection-address=${ROUTER_1_IP} --connection-port=${PORT_1} --connection-id=1 --time 60
```

```sh
# Engine Node
# Host-3
source install/setup.bash
./install/amlip/bin/AML_IP_DL --connection-address="127.0.0.1" --connection-port=${PORT_3_x} --listening-address=${ROUTER_3_IP} --listening-port=${PORT_3} -l 20 -s 10 -p 3 --id=3
```

More nodes could be added to the communication network, but remember that they must configure the portforwarding in their routers to be able to receive data.

### TCP

The *TCP* executables works with similar examples as the explained, but they do not remain in a Discovery Server but in knowing each other.

```sh
# DL Node with TCP
# Host-1
source install/setup.bash
./install/amlip/bin/AML_IP_DL_TCP --connection-address=${ROUTER_2_IP} --connection-port=${PORT_2} --listening-address=${ROUTER_1_IP} --listening-port=${PORT_1}
```

```sh
# Engine Node with TCP
# Host-2
source install/setup.bash
./install/amlip/bin/AML_IP_Engine_TCP --connection-address=${ROUTER_1_IP} --connection-port=${PORT_1} --listening-address=${ROUTER_2_IP} --listening-port=${PORT_2}
```

### UPnP

#### UPnP to this host

In order to open a public router port `${LOGIC_PORT}` and forward it to the host that is executing upnp in port `${PHYSICAL_PORT}` during `${TIME}` seconds with descrciption `${DESCP}` use the command

```sh
./build/cppupnp/example/port-forwarding --logical-port ${LOGIC_PORT} --physical-port ${PHYSICAL_PORT} --time ${TIME} --description ${DESCP}
```

#### UPnP to this host

In order to open a public router port `${LOGIC_PORT}` and forward it to internal address `${ADDRESS}` to port `${PHYSICAL_PORT}` during `${TIME}` seconds with descrciption `${DESCP}` use the command

```sh
./build/cppupnp/example/port-forwarding --logical-port ${LOGIC_PORT} --physical-port ${PHYSICAL_PORT} --time ${TIME} --description ${DESCP} --client ${ADDRESS}
```

## Prototype limits

1. Data sent is randomly generated, with no real meaning for an AML execution.
1. *TCP* version only allows to connect two nodes.

