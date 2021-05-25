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

# Macro to find all thirdparty libraries.
#
# Arguments:
#   :package: The name of the package to find. Used for find_package(${package})
#   :thirdparty_name: The name of the package directory under thirdparty, i.e. thirdparty/${thirdparty_name}
#   :VERSION: [Optional] The minimum required version of the package.
#
# Related CMake options:
#   :THIRDPARTY: Activate the use of internal thirdparties [Defaults: OFF]. Set to ON if EPROSIMA_BUILD is set to ON.
#        Possible values:
#            * ON -> Use submodules only if package is not found elsewhere in the system.
#            * OFF -> Do not use submodules.
#            * FORCE -> Force the use submodules regarless of what is installed in the system.
#   :THIRDPARTY_${package}: Specialization of THIRDPARTY for a specific ${package}. Same possibilities as THIRDPARTY.
#        Unless specified otherwise, it has the same value as THIRDPARTY. It's value has preference over that of
#        THIRDPARTY.
#   :THIRDPARTY_UPDATE: Activate the auto update of internal thirdparties [Defaults: ON]. Possible values: ON/OFF.
#
# The macro's procedure is as follows:
#   1. Try to find the package with find_package.
#         1.1. This step is not taken if THIRDPARTY_${package} is set to FORCE. This happens when the user specifically
#              sets THIRDPARTY_${package} to FORCE, or when THIRDPARTY is set to FORCE and THIRDPARTY_${package} is
#              unspecified (which means it takes the value of THIRDPARTY).
#         1.2. This step is not taken for the case of windows installer. That is if EPROSIMA_INSTALLER is set to ON and
#              at least one of MSVC, MSVC_IDE is set to ON at the same time.
#   2. If the package is not found in 1), and at least one of THIRDPARTY, THIRDPARTY_${package} is set
#      to ON or FORCE, use the thirdparty version.
#         2.1. If THIRDPARTY_UPDATE is set to ON, then update the corresponding git submodule.
#         2.2. Append the thirdparty source directory to CMAKE_PREFIX_PATH.
#         2.3. Try to find the package again.
#   3. If the package was not found anywhere, then print an FATAL_ERROR message.
macro(eprosima_find_thirdparty package thirdparty_name)
    # Parse arguments.
    set(oneValueArgs VERSION)
    cmake_parse_arguments(FIND "" "${oneValueArgs}" "" ${ARGN})
    # Define a list of allowed values for the options THIRDPARTY and THIRDPARTY_${package}
    set(ALLOWED_VALUES ON OFF FORCE)

    # Create the THIRDPARTY variable defaulting to OFF
    set(THIRDPARTY OFF CACHE STRING "Activate use of internal submodules.")
    # Define list of values GUI will offer for the variable
    set_property(CACHE THIRDPARTY PROPERTY STRINGS ON OFF FORCE)
    # Check that specified value is allowed
    if(NOT THIRDPARTY IN_LIST ALLOWED_VALUES)
        message(FATAL_ERROR, "Wrong configuration of THIRDPARTY. Allowed values: ${ALLOWED_VALUES}")
    endif()

    # Create the THIRDPARTY_${package} variable defaulting to ${THIRDPARTY}. This way, we only need to check the value
    # of THIRDPARTY_${package} from here on.
    set(THIRDPARTY_${package} ${THIRDPARTY} CACHE STRING "Activate use of internal submodule ${package}.")
    # Define list of values GUI will offer for the variable
    set_property(CACHE THIRDPARTY_${package} PROPERTY STRINGS ON OFF FORCE)
    # Check that specified value is allowed
    if(NOT THIRDPARTY_${package} IN_LIST ALLOWED_VALUES)
        message(FATAL_ERROR, "Wrong configuration of THIRDPARTY_${package}. Allowed values: ${ALLOWED_VALUES}")
    endif()

    option(THIRDPARTY_UPDATE "Activate the auto update of internal thirdparties" ON)

    # 1. If THIRDPARTY_${package} is set to FORCE, don't try to find the library outside thirdparty.
    # 2. For the case of Windows installer, we don't want to look for the package outside thirdparty. This way we
    #    use thirdparty, meaning we have more control over what is built.
    if((NOT (THIRDPARTY_${package} STREQUAL "FORCE")) AND (NOT (MSVC OR MSVC_IDE)))
        # Try to quietly find the package outside thridparty first.
        find_package(${package} QUIET)

        # Show message if package is found here.
        if(${package}_FOUND)
            # Cannot state where the package is. Asio sets Asio_DIR to Asio_DIR-NOTFOUND even when found.
            message(STATUS "Found ${package}")
        endif()
    endif()

    # Use thirdparty if THIRDPARTY_${package} is set to FORCE, or if the package is not found elsewhere and
    # THIRDPARTY_${package} is set to ON.
    if((THIRDPARTY_${package} STREQUAL "FORCE") OR ((NOT ${package}_FOUND) AND (THIRDPARTY_${package} STREQUAL "ON")))
        if(THIRDPARTY_UPDATE)
            # Update submodule
            message(STATUS "Updating submodule thirdparty/${thirdparty_name}")
            execute_process(
                COMMAND git submodule update --recursive --init "thirdparty/${thirdparty_name}"
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                RESULT_VARIABLE EXECUTE_RESULT
                )
            # A result different than 0 means that the submodule could not be updated.
            if(NOT EXECUTE_RESULT EQUAL 0)
                message(FATAL_ERROR "Cannot configure Git submodule ${package}")
            endif()
        endif()

        # Prepend CMAKE_PREFIX_PATH with the package thirdparty directory. The second path is needed for asio, since the
        # directory is "thirdparty/asio/asio"
        set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/thirdparty/${thirdparty_name} ${CMAKE_PREFIX_PATH})
        set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/thirdparty/${thirdparty_name}/${thirdparty_name} ${CMAKE_PREFIX_PATH})
        find_package(${package} REQUIRED)
    endif()

    # If the package was not found throw an error.
    if(NOT ${package}_FOUND)
        message(FATAL_ERROR "Cannot find package ${package}")
    endif()
endmacro()
