# Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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
# CMake build rules for Fast DDS Monitor
###############################################################################
cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5...3.13)

###############################################################################
# Project
###############################################################################
project(amlip)

###############################################################################
# Load external projects
###############################################################################
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

if(${fastcdr_FOUND})
    message(STATUS "Fast CDR found!")
endif()
if(${fastrtps_FOUND})
    message(STATUS "Fast DDS found!")
endif()

###############################################################################
# Set C++14 as the C++ standard required to build the project
###############################################################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################################################
# Warning level
###############################################################################
if(MSVC OR MSVC_IDE)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
else()
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-unknown-pragmas -Wno-error=deprecated-declarations")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-undefined,error")
    endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

###############################################################################
# Installation paths
###############################################################################
set(BIN_INSTALL_DIR bin/ CACHE PATH "Installation directory for binaries")
set(INCLUDE_INSTALL_DIR include/ CACHE PATH "Installation directory for C++ headers")
set(LIB_INSTALL_DIR lib${LIB_SUFFIX}/ CACHE PATH "Installation directory for libraries")
set(DATA_INSTALL_DIR share/ CACHE PATH "Installation directory for data")
if(WIN32)
    set(LICENSE_INSTALL_DIR . CACHE PATH "Installation directory for licenses")
else()
    set(LICENSE_INSTALL_DIR ${DATA_INSTALL_DIR}/${PROJECT_NAME} CACHE PATH "Installation directory for licenses")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

###############################################################################
# Create executables
###############################################################################
set(TYPES_SOURCES_CXX
    src/cpp/types/Atomization/Atomization.cxx
    src/cpp/types/Atomization/AtomizationPubSubTypes.cxx
    src/cpp/types/DLOutput/DLOutput.cxx
    src/cpp/types/DLOutput/DLOutputPubSubTypes.cxx
    src/cpp/utils/utils.cpp)

# AML-DL executable
message(STATUS "Creating AML_IP_DL executable...")
set(AML_IP_DL_SOURCE
    src/cpp/DL/DLParticipant.cpp
    src/cpp/DL/DL_main.cpp)

add_executable(AML_IP_DL ${TYPES_SOURCES_CXX} ${AML_IP_DL_SOURCE})
target_compile_definitions(AML_IP_DL PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>)

target_link_libraries(AML_IP_DL fastrtps fastcdr foonathan_memory)
install(TARGETS AML_IP_DL
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})

# Engine executable
message(STATUS "Creating AML_IP_Engine executable...")
set(AML_IP_Engine_SOURCE
    src/cpp/Engine/EngineParticipant.cpp
    src/cpp/Engine/Engine_main.cpp)

add_executable(AML_IP_Engine ${TYPES_SOURCES_CXX} ${AML_IP_Engine_SOURCE})
target_compile_definitions(AML_IP_Engine PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>)

target_link_libraries(AML_IP_Engine fastrtps fastcdr foonathan_memory)
install(TARGETS AML_IP_Engine
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})

# Discovery Server executable
message(STATUS "Creating AML_IP_DiscoveryServer executable...")
set(AML_IP_DiscoveryServer_SOURCE
    src/cpp/DiscoveryServer/DiscoveryServerParticipant.cpp
    src/cpp/DiscoveryServer/DiscoveryServer_main.cpp)

add_executable(AML_IP_DiscoveryServer ${TYPES_SOURCES_CXX} ${AML_IP_DiscoveryServer_SOURCE})
target_compile_definitions(AML_IP_DiscoveryServer PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>)

target_link_libraries(AML_IP_DiscoveryServer fastrtps fastcdr foonathan_memory)
install(TARGETS AML_IP_DiscoveryServer
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})

# TCP Direct Connection executables

# AML-DL-TCP executable
message(STATUS "Creating AML_IP_DL_TCP executable...")
set(AML_IP_DL_TCP_SOURCE
    src/cpp/TCPconnection/DL/DLParticipant.cpp
    src/cpp/TCPconnection/DL/DL_main.cpp)

add_executable(AML_IP_DL_TCP ${TYPES_SOURCES_CXX} ${AML_IP_DL_TCP_SOURCE})
target_compile_definitions(AML_IP_DL_TCP PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>)

target_link_libraries(AML_IP_DL_TCP fastrtps fastcdr foonathan_memory)
install(TARGETS AML_IP_DL_TCP
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})

# Engine executable
message(STATUS "Creating AML_IP_Engine_TCP executable...")
set(AML_IP_Engine_TCP_SOURCE
    src/cpp/TCPconnection/Engine/EngineParticipant.cpp
    src/cpp/TCPconnection/Engine/Engine_main.cpp)

add_executable(AML_IP_Engine_TCP ${TYPES_SOURCES_CXX} ${AML_IP_Engine_TCP_SOURCE})
target_compile_definitions(AML_IP_Engine_TCP PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>)

target_link_libraries(AML_IP_Engine_TCP fastrtps fastcdr foonathan_memory)
install(TARGETS AML_IP_Engine_TCP
    RUNTIME DESTINATION ${BIN_INSTALL_DIR})
