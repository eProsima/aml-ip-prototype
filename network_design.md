
# NETWORK DESIGN

## Topics

1. `aml/status`
    1. Publish the id, kind and status of each node
        1. Id : unique inside the network
        1. Kind: Node kind
        1. Status: Enable, Disable

1. `aml/task/request`
    1. Publish a request for computing a task

1. `aml/task/reply_available`
    1. Answers a request for computing a task

1. `aml/task/data`
    1. Task information to process

1. `aml/task/solution`
    1. Data solution from process a task

1. `aml/inference/request`
    1. Publish a request for inference classification from data

1. `aml/inference/reply_available`
    1. Answers a request for inference classification from data

1. `aml/inference/data`
    1. Inference data to process

1. `aml/inference/solution`
    1. Data solution from an inference

## Topic Types

1. `aml::status`
    1. Id
    1. Node kind
    1. Status

1. `aml::request`
    1. Id node
    1. Request id

1. `aml::reply`
    1. Target
    1. Id node
    1. Request id

1. `aml::task::data`
    1. Id node
    1. Request id
    1. Target
    1. Task data

1. `aml::task::solution`
    1. Id node
    1. Request id
    1. Target
    1. Solution data

1. `aml::inference::data`
    1. Id node
    1. Request id
    1. Target
    1. Inference data

1. `aml::inference::solution`
    1. Id node
    1. Request id
    1. Target
    1. Solution data

## Topic kind

1. Public_Request
    1. It publish a message asking for an available Server of specific kind.
    1. QoS
        1. Volatile
        1. Best effort
        1. Lifespan active

1. Direct message
    1. Message between a publisher that is targeted to only one subscriber.
    1. It publish a message with a field that refers to the target entity.
    1. Qos
        1. Volatile
        1. Reliable

1. Update
    1. Update some information that was published before (or for the first time).
    1. Only the last update is the valid data.
        1. Transient local
        1. Reliable

1. Discovery
    1. DDS Discovery data

## Nodes

1. AML-IP Discovery Node
    1. Allow discovery along WAN communications
    1. DDS Router Discovery Tool

1. AML-IP Agent Node
    1. Allow to communicate nodes along WAN
    1. DDS Router Tool

1. AML-IP-Alg Main Node
    1. Task producer.
    1. Inference server

1. AML-IP-Alg Computational Node
    1. Task server.

1. AML-IP Edge Node
    1. Inference producer.
    1. Using ROS2.

## Architecture

| Topic Name                    | Topic Type               | Topic Kind     | Node Publish  | Node Subscribe |
|-------------------------------|--------------------------|----------------|---------------|----------------|
| aml/status                    | aml::status              | Update         | *             | Computational  |
| aml/task/request              | aml::request             | Alarm          | Main          | Computational  |
| aml/task/reply_available      | aml::reply               | Direct Message | Computational | Main           |
| aml/task/data                 | aml::task::data          | Direct Message | Main          | Computational  |
| aml/task/solution             | aml::task::solution      | Direct Message | Computational | Main           |
| aml/inference/request         | aml::request             | Alarm          | Edge          | Main           |
| aml/inference/reply_available | aml::reply               | Direct message | Main          | Edge           |
| aml/inference/data            | aml::inference::data     | Direct message | Edge          | Main           |
| aml/inference/solution        | aml::inference::solution | Direct message | Main          | Edge           |
