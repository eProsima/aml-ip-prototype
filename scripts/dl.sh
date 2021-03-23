#!/bin/bash

# Script that executes a DL AML-IP Participant

# Argument 0 : path to the AML-IP workspace
WORKSPACE=${1}

# Argument 1 : port to connect as TCP client (-1 to use UDP)
CONNECTION_PORT=${2}

# Argument 2 : address to connect as TCP client
CONNECTION_ADDRESS=${3}

echo "--------------------------------------"
echo "Executing DL Participant as TCP client"
echo "Connect to ${CONNECTION_ADDRESS}:${CONNECTION_PORT}"

${WORKSPACE}/build/AML_IP_Prototype/AML_IP_DL --connection-port=${CONNECTION_PORT} --connection-address=${CONNECTION_ADDRESS} | tee aml_dl_result.log
