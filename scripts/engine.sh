#!/bin/bash

# Script that executes an Engine AML-IP Participant

# Argument 0 : path to the AML-IP workspace
WORKSPACE=${1}

# Argument 1 : port to connect as TCP server (-1 to use UDP)
LISTENING_PORT=${2}

# Argument 2 : address to connect as TCP server
LISTENING_ADDRESS=${3}

echo "--------------------------------------"
echo "Executing Engine Participant as TCP server"
echo "Connect to ${LISTENING_ADDRESS}:${LISTENING_PORT}"

${WORKSPACE}/build/AML_IP_Prototype/AML_IP_Engine --listening-port=${LISTENING_PORT} --listening-address=${LISTENING_ADDRESS} | tee aml_engine_result.log
