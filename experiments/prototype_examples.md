
# Prototype examples

## TCP version

### Default transports in localhost

Use Fast-DDS default transports to discovery via Multicast UDP.
The communication is done via intraprocess.

```sh
./AML_IP_Engine_TCP --listening-port=-1
```

```sh
./AML_IP_DL_TCP --connection-port=-1
```

### TCP in localhost

Use `localhost (127.0.0.1:5100)` executable default address to use TCP localhost to communicate to each other.

```sh
./AML_IP_Engine_TCP
```

```sh
./AML_IP_DL_TCP
```

### TCP in LAN

```sh
./AML_IP_Engine_TCP
```

```sh
./AML_IP_DL_TCP
```
