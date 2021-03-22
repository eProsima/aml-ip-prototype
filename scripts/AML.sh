#!/bin/bash

# Script that executes a DL AML-IP Participant and an Engine AML-IP Participant
# The Engine will receive the data from the DL and will send its own data

# Argument 0 : path to the AML-IP workspace
WORKSPACE=${1}

${WORKSPACE}/build/AML_IP_Prototype/AML_IP_DL &
${WORKSPACE}/build/AML_IP_Prototype/AML_IP_Engine
