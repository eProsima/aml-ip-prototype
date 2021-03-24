
# Testing Proof Of Concept 1

Brief:
This test executes a DL Participant with a locator given by args (default, TCP client, TCP server, TCP client and server)
This test executes a Engine Participant with a locator given by args (default, TCP client, TCP server, TCP client and server)


## Architecture configuration

```s
ROUTER ASUS (R1)
router IP  : 192.168.1.131
router LAN : 192.168.42.1
admin : eProsima
psw   : Elbrus
wifi psw   : Micro-ROS
port forwarding : 5200 : 192.168.42.175:5100 (*)

ROUTER RASPI (R2)
router IP  : 192.168.1.20
router LAN : 192.168.20.1
admin : admin
psw   : eProsima2019
wifi psw   : Micro-ROS
DHCP  : redirect every connection to the machines below

RASPBERRY PI PARIS (M1)
IP    : 192.168.42.175
user-name : AML-IP
user  : amlip
psw   : Annapurna

RASPBERRY PI FARM (M2)
IP    : 192.168.20.15
user-name : ip
user  : ip
psw   : raspberry
```

## Test with default transports

### Commands

```sh
# amlip@amlip:~/workspace/amlip
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=-1

# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --listening-port=-1
```

### Results

There is no connection, as expected.

## Test with TCP transport without port forwarding

### Commands

```sh
# amlip@amlip:~/workspace/amlip
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --listening-port=5300 --listening-address=192.168.1.131

# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5300 --connection-address=192.168.1.131
```

### Results

There is no connection, as expected.

## Test with TCP transport and port forward (*)

### Commands

```sh
# amlip@amlip:~/workspace/amlip
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --listening-port=5200 --listening-address=192.168.1.131

# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5200 --connection-address=192.168.1.131
```

and

```sh
# amlip@amlip:~/workspace/amlip
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=-1 --listening-port=5200 --listening-address=192.168.1.131

# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --listening-port=-1 --connection-port=5200 --connection-address=192.168.1.131
```

### Results

There is connection between both endpoints.
It takes few seconds to do the connection, it is not instantaneous.
