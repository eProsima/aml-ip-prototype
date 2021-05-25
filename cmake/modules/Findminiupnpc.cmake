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

message(STATUS "Finding miniupnpc")

find_path(MINIUPNPC_INCLUDE_DIR NAMES miniupnpc.h PATH_SUFFIXES miniupnpc)
find_library(MINIUPNPC_LIBRARY miniupnpc)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(miniupnpc DEFAULT_MSG MINIUPNPC_INCLUDE_DIR MINIUPNPC_LIBRARY)
MARK_AS_ADVANCED(MINIUPNPC_INCLUDE_DIR MINIUPNPC_LIBRARY)

message(STATUS "Miniupnpc headers found: ${MINIUPNPC_INCLUDE_DIR}")
message(STATUS "Miniupnpc library found: ${MINIUPNPC_LIBRARY}")
