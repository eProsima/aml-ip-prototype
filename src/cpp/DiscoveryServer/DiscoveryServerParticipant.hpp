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
 * @file DiscoveryServerParticipant.hpp
 *
 */

#ifndef DiscoveryServerPARTICIPANT_HPP
#define DiscoveryServerPARTICIPANT_HPP

#include "../types/DLOutput/DLOutput.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <fastrtps/participant/Participant.h>

#include <atomic>

#define SERVER_GUID_PREFIX "01.0f.2d.41.4c.47.45.42.52.41.49.43"

class DiscoveryServerListener;

class DiscoveryServerParticipant
{

public:

    DiscoveryServerParticipant();

    virtual ~DiscoveryServerParticipant();

    //!Initialize
    bool init(
        int tcp_port,
        int udp_port,
        std::string address);

    //!Run for number samples
    void run(uint32_t time);

protected:

    void runThread();

private:

    eprosima::fastdds::dds::DomainParticipant* participant_;

    std::string address_;

    int tcp_port_;
};

// TODO check if DS allows to implement with a Listener
// class DiscoveryServerListener : public eprosima::fastdds::dds::DomainParticipantListener
// {
// public:

//     DiscoveryServerListener()
//     {
//     }

//     ~DiscoveryServerListener() override
//     {
//     }

//     void on_publication_matched(
//             eprosima::fastdds::dds::DataWriter* writer,
//             const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

// };

#endif /* DiscoveryServerPARTICIPANT_HPP */
