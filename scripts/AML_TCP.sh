#!/bin/bash

# Script that executes a DL AML-IP Participant and an Engine AML-IP Participant using TCP
# The Engine will receive the data from the DL and will send its own data

# Argument 0 : path to the AML-IP workspace
WORKSPACE=${1}

# Execute both sides of AML
${WORKSPACE}/build/AML_IP_Prototype/AML_IP_DL | tee aml_tcp_dl_result.log &
${WORKSPACE}/build/AML_IP_Prototype/AML_IP_Engine | tee aml_tcp_engine_result.log
