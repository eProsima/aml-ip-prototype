#!/bin/bash

# Script that executes a DL AML-IP Participant and an Engine AML-IP Participant
# The Engine will receive the data from the DL and will send its own data

# Argument 0 : path to the AML-IP workspace
WORKSPACE=${1}

# Set ports to -1 to use UDP and intraprocess
${WORKSPACE}/build/AML_IP_Prototype/AML_IP_DL --connection-port=-1 --listening-port=-1 | tee aml_udp_dl_result.log &
${WORKSPACE}/build/AML_IP_Prototype/AML_IP_Engine --connection-port=-1 --listening-port=-1 | tee aml_udp_engine_result.log
