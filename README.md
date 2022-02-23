# aml-ip-prototype

This repository containes two subprojects with the prototype version for D6.2 in ALMA project (H2020) implementing
the AML-IP workload distributed scenario.

The two subprojets are:

- **amlip_types**: Contains the DDS data types required for the project to communicate nodes.
- **amlip_nodes**: Contains the implementation of the nodes.

## Build and install

```bash
# Change directory to the location where the colcon workspace will be created
cd <path_to_ws>
# Create workspace directory
mkdir -p amlip/src
cd amlip
# Get workspace setup file
wget https://raw.githubusercontent.com/eProsima/aml-ip-prototype/main/amlip.repos
# Download repositories
vcs import src < amlip.repos
# Build the workspace
colcon build
```

## Execution

There are 3 kind of nodes that could be run within this project.
These are the nodes kind and the command to execute any of them (remember to source the workspace installation):

- *AML-IP Status Node*: It shows in real time the status of the different nodes running.
  - `./install/amlip_nodes/lib/amlip_nodes/amlip_status`

- *AML-IP Main Node*: It creates with random period of times random job data and send them distributedly to
  a server to obtain the solution:
  - `./install/amlip_nodes/lib/amlip_nodes/amlip_main`

- *AML-IP Computing Node*: It waits for a job from a client, solve it in a period of random time, and return
  the solution to the client:
  - `./install/amlip_nodes/lib/amlip_nodes/amlip_computing`

### Execute tests

The *amlip_types* subproject does does not have any tests, as their files are `.idl` and autogenerated files.
In order to run the tests for the *amlip_nodes* subproject, execute the following command:

```sh
colcon test --packages-select amlip_nodes
```
