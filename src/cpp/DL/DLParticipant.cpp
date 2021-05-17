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
#include "../utils/utils.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/topic/qos/TopicQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastrtps/xmlparser/XMLProfileManager.h>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>

#include <stdlib.h>
#include <atomic>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

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
        std::string connection_address,
        std::string listening_address)
{
    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos = get_node_qos("DL Participant", connection_address, listening_address);

    participant_ = DomainParticipantFactory::get_instance()->create_participant(DEFAULT_DOMAIN, pqos);

    if (participant_ == nullptr)
    {
        return false;
    }

    std::cout << "DL Participant created with guid: " << participant_->guid().guidPrefix << std::endl
              << " listening in addresses: " << std::endl
              << print_locator(listening_address) // << std::endl // added in print_locator
              << " connecting with servers in addresses: " << std::endl
              << print_ds_locator(connection_address); // << std::endl // added in print_locator

    //REGISTER THE TYPE
    type_.register_type(participant_);

    //CREATE THE PUBLISHER
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT);

    if (publisher_ == nullptr)
    {
        return false;
    }

    //CREATE THE TOPIC
    topic_ = participant_->create_topic(DL_TOPIC, DL_TOPIC_TYPE, TOPIC_QOS_DEFAULT);

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
    // AML_INTEGRATION
    int index = 0;
    while (!stop_.load() && (index < samples || samples == 0))
    {
        // It sleeps to simulate DL execution
        // AML_INTEGRATION change for a condition variable that activates with new data to send
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));

        // In case stop has been pressed between sleep
        if (stop_.load())
        {
            break;
        }

        // Generate random data
        // AML_INTEGRATION change with the data that must be sent
        AML_IP_DLOutput data = generate_random_dloutput_data(data_size);

        if (!publish(data))
        {
            std::cout << std::endl << "<< DL Participant " << participant_->guid().guidPrefix
                      << " ERROR sending message: " << ++index << std::endl;
        }
        else
        {
            std::cout << std::endl << "<< DL Participant " << participant_->guid().guidPrefix
                      << " sent DLOutput number: " << ++index << " message: " << data << std::endl;
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
        std::cout << "DL Participant " << participant_->guid().guidPrefix
                  << " publishing. Please press enter to stop it at any time." << std::endl;
        std::cin.ignore();
        stop_.store(true);
    }
    else
    {
        std::cout << "DL Participant " << participant_->guid().guidPrefix
                  << " publishing " << samples << " samples." << std::endl;
    }

    thread.join();
}

bool DLParticipant::publish(
        AML_IP_DLOutput data)
{
    return writer_->write((void*)&data);
}

void DLListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter* writer,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        std::cout << "DL Participant " << writer->guid().guidPrefix << " matched with "
                  << info.last_subscription_handle << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "DL Participant " << writer->guid().guidPrefix << " unmatched with "
                  << info.last_subscription_handle << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
}
