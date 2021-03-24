
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

ROUTER ANNAPURNA
router IP  : 192.168.1.132
router LAN : 192.168.2.x
admin : eProsima
psw   : Micro-ROS
wifi psw   : Micro-ROS

ROUTER FARM
router IP  : 192.168.1.20
router LAN : 192.168.20.x
admin : admin
psw   : eProsima2019
wifi psw   : Micro-ROS

RASPBERRY ELBRUS
IP    : 192.168.42.175
user-name : elbrus-raspo
user  : ip
psw   : Elbrus

RASPBERRY ELBRUS
IP    : 192.168.2.50
user-name : annapurna-raspo
user  : ip
psw   : Annapurna

RASPBERRY FARM
IP    : 192.168.20.15
user-name : ip
user  : ip
psw   : raspberry
```
