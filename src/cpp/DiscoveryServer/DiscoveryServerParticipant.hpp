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
#include "../types/types.hpp"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/rtps/participant/ParticipantDiscoveryInfo.h>

#include <fastrtps/participant/Participant.h>

#include <atomic>

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
        std::string address,
        bool backup);

    //!Run for number samples
    void run(uint32_t time);

protected:

    void runThread();

private:

    eprosima::fastdds::dds::DomainParticipant* participant_;

    std::string address_;

    DiscoveryServerListener* listener_;

    int tcp_port_;
};

class DiscoveryServerListener : public eprosima::fastdds::dds::DomainParticipantListener
{
public:

    DiscoveryServerListener()
    {
    }

    ~DiscoveryServerListener() override
    {
    }

    void on_participant_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info) override;

    void on_subscriber_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::ReaderDiscoveryInfo&& info) override;

    void on_publisher_discovery(
            eprosima::fastdds::dds::DomainParticipant* participant,
            eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info) override;
};

#endif /* DiscoveryServerPARTICIPANT_HPP */
