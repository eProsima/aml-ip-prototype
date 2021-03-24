
# Testing Docker communication

Brief:
This test creates a Docker Image with the src repo of the prototype.
It is tested different transports between the Docker and the host machine.

## Architecture configuration

```s
DOCKER
IP: `172.17.0.2`

HOST
IP: `192.168.1.64`
```

## Test with default transports

### Commands

```sh
# Docker
./build/AML_IP_Prototype/AML_IP_DL --connection-port -1

# Host
./usr/local/bin/AML_IP_Engine --listening-port -1
```

### Results

There is connection.

## Test with TCP transport

### Commands

```sh
# Docker
./usr/local/bin/AML_IP_Engine --listening-port=5800 --listening-address="172.17.0.2"

# Host
./build/AML_IP_Prototype/AML_IP_DL --connection-port=5800 --connection-address="172.17.0.2"
```

and

```sh
# Docker
./usr/local/bin/AML_IP_DL --connection-address="192.168.1.64"

# Host
./build/AML_IP_Prototype/AML_IP_Engine --listening-address "192.168.1.64"
```

### Results

There is connection.
