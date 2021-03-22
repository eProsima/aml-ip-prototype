#!/bin/bash

# Script that executes an Engine AML-IP Participant

# Argument 0 : path to the AML-IP workspace
WORKSPACE = ${1}
${WORKSPACE}/build/AML_IP_Prototype/AML_IP_Engine
