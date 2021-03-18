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
    , dl_type_(new AML_IP_DLOutput()
    , atomization_type_(new AML_IP_Atomization()
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
    pqos.name("Engine Participant");

    descriptor->sendBufferSize = 0;
    descriptor->receiveBufferSize = 0;

    participant_ = DomainParticipantFactory::get_instance()->create_participant(domain, pqos);

    if (participant_ == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPES
    dl_type_.register_type(participant_);
    atomization_type_.register_type(participant_);

    //CREATE THE PUBLISHER
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT);

    if (publisher_ == nullptr)
    {
        return false;
    }

    //CREATE THE SUBSCRIBER
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT);

    if (subscriber == nullptr)
    {
        return false;
    }

    //CREATE THE DL TOPIC
    dl_topic_ = participant_->create_topic("_aml_ip_topic_dloutput", "AML_IP_DLOutput", TOPIC_QOS_DEFAULT);

    if (dl_topic_ == nullptr)
    {
        return false;
    }

    //CREATE THE ATOM TOPIC
    atomization_topic_ = participant_->create_topic("_aml_ip_topic_atomization", "AML_IP_Atomization", TOPIC_QOS_DEFAULT);

    if (atomization_topic_ == nullptr)
    {
        return false;
    }

    //CREATE THE WRITER LISTENER
    writer_listener_ = new EngineWriterListener();

    //CREATE THE DATAWRITER
    writer_ = publisher_->create_datawriter(atomization_topic_, DATAWRITER_QOS_DEFAULT, writer_listener_);

    if (writer_ == nullptr)
    {
        return false;
    }

    //CREATE THE READER LISTENER
    dl_reader_listener_ = new DLReaderListener();

    //CREATE THE DATAREADER DL
    dl_reader_ = publisher_->create_datareader(dl_topic_, DATAREADER_QOS_DEFAULT, dl_reader_listener_);

    if (dl_reader_ == nullptr)
    {
        return false;
    }

    //CREATE THE READER LISTENER
    atomization_reader_listener_ = new AtomizationReaderListener();

    atomization_reader_ = publisher_->create_datareader(
        atomization_topic_,
        DATAREADER_QOS_DEFAULT,
        atomization_reader_listener_);

    if (atomization_reader_ == nullptr)
    {
        return false;
    }

    return true;
}

HelloWorldPublisher::~HelloWorldPublisher()
{
    // Delete listeners
    if (writer_listener_ != nullptr)
    {
        writer_.set_listener(nullptr);
        delete writer_listener_;
    }
    if (dl_reader_listener_ != nullptr)
    {
        dl_reader_.set_listener(nullptr);
        delete dl_reader_listener_;
    }
    if (atomization_reader_listener_ != nullptr)
    {
        atomization_reader_.set_listener(nullptr);
        delete atomization_reader_listener_;
    }

    // Delete endpoints
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (dl_reader_ != nullptr)
    {
        publisher_->delete_datareader(dl_reader_);
    }
    if (atomization_reader_ != nullptr)
    {
        publisher_->delete_datareader(atomization_reader_);
    }

    // Delete entities
    if (subscriber_ != nullptr)
    {
        participant_->delete_subscriber(subscriber_);
    }
    if (publisher_ != nullptr)
    {
        participant_->delete_publisher(publisher_);
    }

    // Delete topics
    if (dl_topic_ != nullptr)
    {
        participant_->delete_topic(dl_topic_);
    }
    if (atomization_topic_ != nullptr)
    {
        participant_->delete_topic(atomization_topic_);
    }

    // Delete participant
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
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
            std::cout << "Engine finish process and sends atomization: " << index << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }
}

void HelloWorldPublisher::run(
        int samples,
        float period)
{
    std::thread thread(&HelloWorldPublisher::runThread, this, samples, static_cast<long>(period * 1000));

    std::cout << "Engine Participant running. Please press enter to stop_ it at any time." << std::endl;
    std::cin.ignore();
    stop_ = true;

    thread.join();
}

bool HelloWorldPublisher::publish(
        bool waitForListener)
{
    AML_IP_Atomization data = generate_random_data_()
    writer_->write((void*)&data);
    return true;
}


void EngineWriterListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter*,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        std::cout << "Engine Participant matched with " << info.last_subscription_handle << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "Engine Participant unmatched with " << info.last_subscription_handle << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
}

void DLReaderListener::on_subscription_matched(
        DataReader*,
        const SubscriptionMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        std::cout << "Engine Participant matched with a new Dl: " << info.last_subscription_handle << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "Engine Participant unmatched with Dl: " << info.last_subscription_handle << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

void DLReaderListener::on_data_available(
        DataReader* reader)
{
    // TODO do not read it if it comes from us
    SampleInfo info;
    AML_IP_DLOutput data;
    if (reader->take_next_sample(&data, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            std::cout << "DL message " << ++samples_ << " received from " << info.instance_state << std::endl;
        }
    }
}


void AtomizationReaderListener::on_subscription_matched(
        DataReader*,
        const SubscriptionMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        std::cout << "Engine Participant matched with other Engine: " << info.last_subscription_handle << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "Engine Participant unmatched with Engine: " << info.last_subscription_handle << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

void AtomizationReaderListener::on_data_available(
        DataReader* reader)
{
    // TODO do not read it if it comes from us
    SampleInfo info;
    AML_IP_DLOutput data;
    if (reader->take_next_sample(&data, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            std::cout << "Atomization message " << ++samples_ << " received from " << info.instance_state << std::endl;
        }
    }
}
