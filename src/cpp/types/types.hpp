// Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file types.hpp
 *
 */

#ifndef TYPES_TYPES_HPP
#define TYPES_TYPES_HPP

#include <sstream>

#include <fastdds/rtps/common/GuidPrefix_t.hpp>

#define SERVER_DEFAULT_GUID "01.0f.00.41.4c.47.45.42.52.41.49.43"
#define SERVER_DEFAULT_GUID_ID_INDEX 2
#define DEFAULT_DOMAIN 11

constexpr const char* DL_TOPIC = "_aml_ip_topic_dloutput";
constexpr const char* ENGINE_TOPIC = "_aml_ip_topic_atomization";

constexpr const char* DL_TOPIC_TYPE = "AML_IP_DLOutput";
constexpr const char* ENGINE_TOPIC_TYPE = "AML_IP_Atomization";

inline eprosima::fastrtps::rtps::GuidPrefix_t guid_server(
        int id)
{
    eprosima::fastrtps::rtps::GuidPrefix_t guid;
    std::istringstream(SERVER_DEFAULT_GUID) >> guid;
    guid.value[SERVER_DEFAULT_GUID_ID_INDEX] = static_cast<unsigned char>(id);
    return guid;
}

#endif // TYPES_TYPES_HPP
