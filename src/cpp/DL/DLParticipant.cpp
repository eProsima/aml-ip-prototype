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
 * @file DLParticipant.cpp
 *
 */

#include "DLParticipant.hpp"
#include "../types/DLOutput/DLOutputPubSubTypes.h"
#include "../types/utils/utils.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/topic/qos/TopicQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>

#include <stdlib.h>
#include <atomic>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;

DLParticipant::DLParticipant()
    : participant_(nullptr)
    , publisher_(nullptr)
    , topic_(nullptr)
    , writer_(nullptr)
    , listener_(nullptr)
    , type_(new AML_IP_DLOutputPubSubType())
    , stop_(false)
{
}

bool DLParticipant::init(
        int domain)
{
    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod =
            eprosima::fastrtps::Duration_t(2, 0);
    pqos.name("DL Participant");

    participant_ = DomainParticipantFactory::get_instance()->create_participant(domain, pqos);

    if (participant_ == nullptr)
    {
        return false;
    }

    std::cout << "DL Participant created with guid: " << participant_->guid() << std::endl;

    //REGISTER THE TYPE
    type_.register_type(participant_);

    //CREATE THE PUBLISHER
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT);

    if (publisher_ == nullptr)
    {
        return false;
    }

    //CREATE THE TOPIC
    topic_ = participant_->create_topic("_aml_ip_topic_dloutput", "AML_IP_DLOutput", TOPIC_QOS_DEFAULT);

    if (topic_ == nullptr)
    {
        return false;
    }

    //CREATE THE LISTENER
    listener_ = new DLListener();

    //CREATE THE DATAWRITER
    writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, listener_);

    if (writer_ == nullptr)
    {
        return false;
    }

    return true;
}

DLParticipant::~DLParticipant()
{
    if (listener_ != nullptr)
    {
        writer_->set_listener(nullptr);
        delete listener_;
    }
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (publisher_ != nullptr)
    {
        participant_->delete_publisher(publisher_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void DLParticipant::runThread(
        int samples,
        long sleep_ms,
        uint32_t data_size)
{
    int index = 0;
    while (!stop_.load() && (index < samples || samples == 0))
    {
        // It sleeps to simulate DL execution
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

        // In case stop has been pressed between sleep
        if (stop_.load())
        {
            break;
        }

        // Generate randome data
        AML_IP_DLOutput data = generate_random_dloutput_data(data_size);

        if (!publish(data))
        {
            std::cout << "ERROR sending message: " << ++index << std::endl;
        }
        else
        {
            std::cout << "DL Participant sent DLOutput number: " << ++index
                << " message: " << data << std::endl << std::endl;
        }
    }
}

void DLParticipant::run(
        int samples,
        float period,
        uint32_t data_size)
{
    std::thread thread(&DLParticipant::runThread, this, samples, static_cast<long>(period * 1000), data_size);

    if (samples == 0)
    {
        std::cout << "DL Participant publishing. Please press enter to stop it at any time." << std::endl;
        std::cin.ignore();
        stop_.store(true);
    }
    else
    {
        std::cout << "DL Participant publishing " << samples << " samples." << std::endl;
    }

    thread.join();
}

bool DLParticipant::publish(AML_IP_DLOutput data)
{
    return writer_->write((void*)&data);
}

void DLListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter*,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        std::cout << "DL Participant matched with " << info.last_subscription_handle << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "DL Participant unmatched with " << info.last_subscription_handle << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
}
