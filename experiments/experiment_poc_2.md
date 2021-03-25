
# Testing Proof Of Concept 2

Brief:
Execute a scenario with 3 different independent LANs
Execute a Discovery Server with TCP transport
Executes a DL Participant that connects to DS and with TCP transport client and server
Executes a Engine Participant that connects to DS and with TCP transport client and server

## Architecture configuration

```s
ROUTER ELBRUS
router IP  : 192.168.1.131
router LAN : 192.168.42.x
admin : eProsima
psw   : Micro-ROS
wifi psw   : Micro-ROS
port forwarding : 5200 -> 192.168.42.175

ROUTER ANNAPURNA
router IP  : 192.168.1.132
router LAN : 192.168.2.x
admin : eProsima
psw   : Micro-ROS
wifi psw   : Micro-ROS
port forwarding : 5300 -> 192.168.2.50

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

Using TCP transport and in 3 different LANs, execute following commands in different raspberrys

### Commands

```sh
# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -t 5100 -a "192.168.20.15" --time 30

# pi@annapurna-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5100 --connection-address="192.168.20.15" --listening-port=5300 --listening-address="192.168.1.132"

# pi@elbrus-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5100 --connection-address="192.168.20.15" --listening-port=5200 --listening-address="192.168.1.131"
```

and

```sh
# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5400 --listening-address="192.168.20.15"

# pi@annapurna-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -t 5300 -a "192.168.1.132" --time 30

# pi@elbrus-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5300 --connection-address="192.168.1.132" --listening-port=5200 --listening-address="192.168.1.131"
```

and

```sh
# 3 Different LANs without static redirecting
# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -t 5100 -a "192.168.1.20" --time 30

# pi@annapurna-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5100 --connection-address="192.168.1.20" --listening-port=5300 --listening-address="192.168.1.132"

# pi@elbrus-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5100 --connection-address="192.168.1.20" --listening-port=5200 --listening-address="192.168.1.131"
```

### Results

They match.
There is communication.

## Raspberrys Configuration commands

```bash
# enter raspi farm and compile last version
ssh pi@192.168.20.15
raspberry
cd amlip/
# these commands may need github user and password
vcs import src < amlip.repos
vcs pull
colcon build --cmake-clean-cache --cmake-args -DCOMPILE_EXAMPLES=ON

# enter raspi annapurna and copy compilation
ssh pi@192.168.1.132 -p 10300
Annapurna
cd Workspace/amlip/
rm -rf install/
scp -r pi@192.168.20.15:/home/pi/amlip/install .
raspberry

# enter raspi elbrus and copy compilation
ssh pi@192.168.1.131 -p 10300
Elbrus
cd Workspace/amlip/
rm -rf install/
scp -r pi@192.168.20.15:/home/pi/amlip/install .
raspberry
```

## TEST LOCAL UDP

Using default and TCP transport in same host, execute following commands
### Commands

```sh
# paris@paris-XPS-15-9570:~/projects/AML-IP$
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -a 127.0.0.1

# paris@paris-XPS-15-9570:~/projects/AML-IP$
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=11811 --connection-address="127.0.0.1" --listening-port=11333 --listening-address="127.0.0.1"

# paris@paris-XPS-15-9570:~/projects/AML-IP$
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=11811 --connection-address="127.0.0.1" --listening-port=11334 --listening-address="127.0.0.1"
```

### Results

They match.
There is communication.


## TEST WAN TCP in closing ports

Using TCP transport and in 3 different LANs, execute following commands in different raspberrys

### Commands

```sh
# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -t 5110 -a "192.168.1.20" --time 30 # change to not open port

# pi@annapurna-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5110 --connection-address="192.168.1.20" --listening-port=5300 --listening-address="192.168.1.132"

# pi@elbrus-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5110 --connection-address="192.168.1.20" --listening-port=5200 --listening-address="192.168.1.131"
```

and

```sh
# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DiscoveryServer -t 5100 -a "192.168.1.20" --time 30

# pi@annapurna-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_DL --connection-port=5100 --connection-address="192.168.1.20" --listening-port=5300 --listening-address="192.168.1.132"

# pi@elbrus-raspi:~/Workspace/amlip $
source install/setup.bash
./install/AML_IP_Prototype/bin/AML_IP_Engine --connection-port=5100 --connection-address="192.168.1.20" --listening-port=5299 --listening-address="192.168.1.131" # change to not open port
```

### Results

They match.
There is no  communication as expected.
