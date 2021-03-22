# Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###############################################################################
# Configuration
###############################################################################
cmake_minimum_required(VERSION 2.8.12)

set(FORCE_CXX "11" CACHE STRING "C++ standard fulfillment selection")

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project("AML_IP_Prototype")

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT foonathan_memory_FOUND)
    find_package(foonathan_memory REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

#Check C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()


###############################################################################
# Installation paths
###############################################################################
set(BIN_INSTALL_DIR bin/ CACHE PATH "Installation directory for binaries")
set(INCLUDE_INSTALL_DIR include/ CACHE PATH "Installation directory for C++ headers")
set(LIB_INSTALL_DIR lib${LIB_SUFFIX}/ CACHE PATH "Installation directory for libraries")
set(DATA_INSTALL_DIR share/ CACHE PATH "Installation directory for data")
set(LICENSE_INSTALL_DIR . CACHE PATH "Installation directory for licenses")

###############################################################################
# Create executables
###############################################################################
set(TYPES_SOURCES_CXX
    src/cpp/types/Atomization/Atomization.cxx
    src/cpp/types/Atomization/AtomizationPubSubTypes.cxx
    src/cpp/types/DLOutput/DLOutput.cxx
    src/cpp/types/DLOutput/DLOutputPubSubTypes.cxx
    src/cpp/types/utils/utils.cpp)

# AML-DL executable
message(STATUS "Configuring AML_IP_DL executable...")
set(AML_IP_DL_SOURCE
    src/cpp/DL/DLParticipant.cpp
    src/cpp/DL/DL_main.cpp)

add_executable(AML_IP_DL ${TYPES_SOURCES_CXX} ${AML_IP_DL_SOURCE})
target_compile_definitions(AML_IP_DL PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    )
target_link_libraries(AML_IP_DL fastrtps fastcdr foonathan_memory)
install(TARGETS AML_IP_DL
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})

# Engine executable
message(STATUS "Configuring AML_IP_Engine executable...")
set(AML_IP_Engine_SOURCE
    src/cpp/Engine/EngineParticipant.cpp
    src/cpp/Engine/Engine_main.cpp)

add_executable(AML_IP_Engine ${TYPES_SOURCES_CXX} ${AML_IP_Engine_SOURCE})
target_compile_definitions(AML_IP_Engine PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG> # Internal debug activated.
    )
target_link_libraries(AML_IP_Engine fastrtps fastcdr foonathan_memory)
install(TARGETS AML_IP_Engine
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})
