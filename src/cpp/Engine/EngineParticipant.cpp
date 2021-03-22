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

#include "EngineParticipant.hpp"
#include "../types/DLOutput/DLOutputPubSubTypes.h"
#include "../types/Atomization/AtomizationPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/subscriber/qos/SubscriberQos.hpp>
#include <fastdds/dds/topic/qos/TopicQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>

#include <atomic>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;

EngineParticipant::EngineParticipant()
    : participant_(nullptr)
    , publisher_(nullptr)
    , subscriber_(nullptr)
    , dl_topic_(nullptr)
    , atomization_topic_(nullptr)
    , writer_(nullptr)
    , dl_reader_(nullptr)
    , atomization_reader_(nullptr)
    , writer_listener_(nullptr)
    , dl_reader_listener_(nullptr)
    , atomization_reader_listener_(nullptr)
    , dl_type_(new AML_IP_DLOutputPubSubType())
    , atomization_type_(new AML_IP_AtomizationPubSubType())
    , stop_(false)
{
}

bool EngineParticipant::init(
        int domain)
{
    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod =
            eprosima::fastrtps::Duration_t(2, 0);
    pqos.name("Engine Participant");

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

    if (subscriber_ == nullptr)
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
    dl_reader_ = subscriber_->create_datareader(
        dl_topic_,
        DATAREADER_QOS_DEFAULT,
        dl_reader_listener_);

    if (dl_reader_ == nullptr)
    {
        return false;
    }

    //CREATE THE READER LISTENER
    atomization_reader_listener_ = new AtomizationReaderListener();

    atomization_reader_ = subscriber_->create_datareader(
        atomization_topic_,
        DATAREADER_QOS_DEFAULT,
        atomization_reader_listener_);

    if (atomization_reader_ == nullptr)
    {
        return false;
    }

    return true;
}

EngineParticipant::~EngineParticipant()
{
    // Delete listeners
    if (writer_listener_ != nullptr)
    {
        writer_->set_listener(nullptr);
        delete writer_listener_;
    }
    if (dl_reader_listener_ != nullptr)
    {
        dl_reader_->set_listener(nullptr);
        delete dl_reader_listener_;
    }
    if (atomization_reader_listener_ != nullptr)
    {
        atomization_reader_->set_listener(nullptr);
        delete atomization_reader_listener_;
    }

    // Delete endpoints
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (dl_reader_ != nullptr)
    {
        subscriber_->delete_datareader(dl_reader_);
    }
    if (atomization_reader_ != nullptr)
    {
        subscriber_->delete_datareader(atomization_reader_);
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

void EngineParticipant::runThread(
        int samples,
        long sleep_ms)
{
    int index = 0;
    while (!stop_ && (index < samples || samples == 0))
    {
        if (!publish())
        {
            std::cout << "ERROR sending message: " << ++index << std::endl;
        }
        else
        {
            std::cout << "Engine finish process and sends atomization: " << ++index << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    }
}

void EngineParticipant::run(
        int samples,
        float period,
        uint32_t data_size)
{
    std::thread thread(&EngineParticipant::runThread, this, samples, static_cast<long>(period * 1000), data_size);

    if (samples == 0)
    {
        std::cout << "Engine Participant publishing " << samples << " samples." << std::endl;
        std::cin.ignore();
        stop_ = true;
    }
    else
    {
        std::cout << "Engine Participant publishing. Please press enter to stop_ it at any time." << std::endl;
    }

    thread.join();
}

bool EngineParticipant::publish(uint32_t data_size)
{
    AML_IP_Atomization data = generate_random_data_(data_size);
    writer_->write((void*)&data);
    return true;
}

AML_IP_Atomization EngineParticipant::generate_random_data_(uint32_t data_size)
{
    AML_IP_Atomization data;
    std::vector<AML_IP_Atom> atoms;

    int atoms_number = rand() % data_size + 1;
    for (int i = 0; i < atoms_number; ++i)
    {
        AML_IP_Atom atom_data;

        // The size of the atoms are always the same
        std::vector<uint32_t> ucs(data_size);
        for (int j = 0; j < data_size; ++j)
        {
            ucs[j] = rand();
        }

        atom_data.ucs(ucs);

        atoms.push_back(atom_data);
    }
    data.atoms(atoms);

    return data;
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
        std::cout << "Engine Participant matched with a new Dl: " << info.last_publication_handle << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "Engine Participant unmatched with Dl: " << info.last_publication_handle << std::endl;
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
        std::cout << "Engine Participant matched with other Engine: " << info.last_publication_handle << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "Engine Participant unmatched with Engine: " << info.last_publication_handle << std::endl;
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
