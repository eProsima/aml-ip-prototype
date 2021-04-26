
# Prototype examples

## TCP version

Version using `*_TCP` executables, that uses static TCP discovery without Discovery Server.

### Default transports in localhost

Use Fast-DDS default transports to discovery via Multicast UDP.
The communication is done via intraprocess.

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DL_TCP --connection-port=-1
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_Engine_TCP  --listening-port=-1
```

### TCP in localhost

Use `localhost (127.0.0.1:5100)` executable default address to use TCP localhost to communicate to each other.

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DL_TCP
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_Engine_TCP
```

### TCP in LAN

Use Fast-DDS default multicast transport to discover nodes.
Once discovered, UDP transport it is used to communicate to each other.

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DL_TCP
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_Engine_TCP
```

### TCP in WAN

```sh
# Elbrus raspi
source install/setup.bash
./install/amlip/bin/AML_IP_Engine_TCP --listening-port=5200 --listening-address=192.168.1.131
```

```sh
# Local
source install/setup.bash
./install/amlip/bin/AML_IP_DL_TCP --connection-port=5200 --connection-address=192.168.1.131
```

## Discovery Server version

Version using std executables, that uses Discovery Server intermediate node to discovery.

### DS in localhost

Use `localhost (127.0.0.1:5100)` executable default address to use TCP localhost to communicate to each other.

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer -t 15
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DL
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_Engine
```

### DS in LAN

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DiscoveryServer -t 15
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DL
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_Engine
```

### DS in WAN

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_DL_TCP
```

```sh
source install/setup.bash
./install/amlip/bin/AML_IP_Engine_TCP
```
