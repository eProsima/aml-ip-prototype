
# Testing Discovery Server Example

In this test it is used the PR <https://github.com/eProsima/Discovery-Server/pull/31>.
The example is set as it is commented in the PR and with the following configuration.

Briefly, this executes a Discovery Server and a Client (publisher or subscriber) in machine1 (M1) connected with
router1 (R1). R1 has a port forwarding for M1 for the discovery server port.
Then, and endpoint starts in other machine2 (M2), connected to a router2 (R2).
R1 and R2 must be independent

## Architecture configuration

```s
ROUTER ASUS (R1)
router IP  : 192.168.1.131
router LAN : 192.168.42.1
admin : eProsima
psw   : Elbrus
wifi psw   : Micro-ROS
port forwarding : 5200 : 192.168.42.175:5100
comments   : only 1 client (raspi 192.168.42.175)

ROUTER RASPI (R2)
router IP  : 192.168.1.20
router LAN : 192.168.20.1
admin : admin
psw   : eProsima2019
wifi psw   : Micro-ROS
DHCP  : redirect every connection to the machines below (*)

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

## Commands

```sh
# amlip@amlip:~/workspace/amlip
source install/setup.bash
./install/discovery-server/examples/C++/HelloWorldExampleDS/HelloWorldExampleDS server --tcp --ip=192.168.42.175:5200 --wan=192.168.1.131
# amlip@amlip:~/workspace/amlip
source install/setup.bash
./install/discovery-server/examples/C++/HelloWorldExampleDS/HelloWorldExampleDS publisher --tcp --ip=192.168.42.175:5200 --wan=192.168.1.131 -c 30

# pi@raspfarm05:~/amlip $
source install/setup.bash
./install/discovery-server/examples/C++/HelloWorldExampleDS/HelloWorldExampleDS subscriber --tcp --ip=192.168.42.175:5200 --wan=192.168.20.15
```

## Results

They got the match and client communicate to each other
This wasn't the result expected, but thanks to the raspi router, the communication with the raspi (farm) was direct and accesible from router (asus)
(*) Thanks to this redirectionning, the connection is viable
