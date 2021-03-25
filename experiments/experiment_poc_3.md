
# Testing Proof Of Concept 3

Brief:
Execute a scenario with 2 different independent LANs
LAN 1 Execute a DL and a Discovery Server
LAN 2 Execute a Engine and a DL

## Architecture configuration

```s
ROUTER ELBRUS
router IP  : 192.168.1.131
router LAN : 192.168.42.x
admin : eProsima
psw   : Micro-ROS
wifi psw   : Micro-ROS
port forwarding : 5200 -> 192.168.42.175:5200
port forwarding : 5210 -> 192.168.42.175:5210

ROUTER ANNAPURNA
router IP  : 192.168.1.132
router LAN : 192.168.2.x
admin : eProsima
psw   : Micro-ROS
wifi psw   : Micro-ROS
port forwarding : 5300 -> 192.168.2.50:5300
port forwarding : 5310 -> 192.168.2.50:5310

ROUTER FARM
router IP  : 192.168.1.20
router LAN : 192.168.20.x
admin : admin
psw   : eProsima2019
wifi psw   : Micro-ROS
DHCP  : each raspberry has static IP
port forwarding : 5100 -> 192.168.20.15

RASPBERRY ELBRUS
IP    : 192.168.42.175
user-name : elbrus-raspo
user  : ip
psw   : Elbrus
ssh pi@192.168.1.131 -p 10300

RASPBERRY ANNAPURNA
IP    : 192.168.2.50
user-name : annapurna-raspo
user  : ip
psw   : Annapurna
ssh pi@192.168.1.132 -p 10300

RASPBERRY FARM
IP    : 192.168.20.15
user-name : ip
user  : ip
psw   : raspberry
ssh pi@192.168.20.15
```

## TEST WAN TCP

Using TCP transport and in 2 different LANs, execute following commands in different raspberrys

### Commands

```sh
# pi@annapurna-raspi:~/Workspace/amlip $
# Terminal 1
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -t 5300 -a "192.168.1.132" --time 45

# Terminal 2
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5310 --listening-address="192.168.1.132"

# pi@elbrus-raspi:~/Workspace/amlip $
# Terminal 1
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5200 --listening-address="192.168.1.131"

# Terminal 2
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5210 --listening-address="192.168.1.131"
```

### Results

They match.
There is communication.

## TEST WAN TCP 3 LAN

Using TCP transport and in 2 different LANs, execute following commands in different raspberrys

### Commands

```sh
# pi@annapurna-raspi:~/Workspace/amlip $
# Terminal 1
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -t 5300 -a "192.168.1.132" --time 45

# Terminal 2
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5310 --listening-address="192.168.1.132"

# pi@elbrus-raspi:~/Workspace/amlip $
# Terminal 1
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5200 --listening-address="192.168.1.131"

# Terminal 2
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5210 --listening-address="192.168.1.131"

# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5432 --listening-address="192.168.20.15"
```

### Results

They match.
There is communication.
