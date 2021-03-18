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

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;

HelloWorldPublisher::HelloWorldPublisher()
    : participant_(nullptr)
    , publisher_(nullptr)
    , topic_(nullptr)
    , writer_(nullptr)
    , type_(new AML_IP_DLOutput()
    , stop_(false))
{
}

bool HelloWorldPublisher::init(
        int domain,
        float period)
{
    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod =
            eprosima::fastrtps::Duration_t(period, 0);
    pqos.name("DL Participant");

    descriptor->sendBufferSize = 0;
    descriptor->receiveBufferSize = 0;

    participant_ = DomainParticipantFactory::get_instance()->create_participant(domain, pqos);

    if (participant_ == nullptr)
    {
        return false;
    }

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

HelloWorldPublisher::~HelloWorldPublisher()
{
    if (listener_ != nullptr)
    {
        writer_.set_listener(nullptr);
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

void HelloWorldPublisher::runThread(
        int samples,
        long sleep_ms)
{
    int index = 0;
    while (!stop_ && (index < samples || samples == 0))
    {
        if (!publish(false))
        {
            std::cout << "ERROR sending message: " << index << std::endl;
        }
        else
        {
            std::cout << "DL Participant sent message: " << index << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }
}

void HelloWorldPublisher::run(
        int samples,
        float period)
{
    std::thread thread(&HelloWorldPublisher::runThread, this, samples, static_cast<long>(period * 1000));

    std::cout << "DL Participant publishing. Please press enter to stop_ it at any time." << std::endl;
    std::cin.ignore();
    stop_ = true;

    thread.join();
}

bool HelloWorldPublisher::publish(
        bool waitForListener)
{
    AML_IP_DLOutput data = generate_random_data_()
    writer_->write((void*)&data);
    return true;
}