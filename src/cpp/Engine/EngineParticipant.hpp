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
 * @file EngineParticipant.hpp
 *
 */

#ifndef ENGINEPARTICIPANT_HPP
#define ENGINEPARTICIPANT_HPP

#include "../types/DLOutput/DLOutput.h"
#include "../types/Atomization/Atomization.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <atomic>

class EngineWriterListener;
class DLReaderListener;
class AtomizationReaderListener;

class EngineParticipant
{

public:

    EngineParticipant();

    virtual ~EngineParticipant();

    //!Initialize
    bool init(
        int domain,
        int connection_port,
        std::string connection_address,
        int listening_port,
        std::string listening_address);

    //!Publish a sample
    bool publish(AML_IP_Atomization data);

    //!Run for number samples
    void run(int samples, float period, uint32_t data_size);

protected:

    void runThread(
            int number,
            long sleep_ms,
            uint32_t data_size);

private:

    eprosima::fastdds::dds::DomainParticipant* participant_;

    eprosima::fastdds::dds::Publisher* publisher_;
    eprosima::fastdds::dds::Subscriber* subscriber_;

    eprosima::fastdds::dds::Topic* dl_topic_;
    eprosima::fastdds::dds::Topic* atomization_topic_;

    eprosima::fastdds::dds::DataWriter* writer_;
    eprosima::fastdds::dds::DataReader* dl_reader_;
    eprosima::fastdds::dds::DataReader* atomization_reader_;

    EngineWriterListener* writer_listener_;
    DLReaderListener* dl_reader_listener_;
    AtomizationReaderListener* atomization_reader_listener_;

    eprosima::fastdds::dds::TypeSupport dl_type_;
    eprosima::fastdds::dds::TypeSupport atomization_type_;

    std::atomic_bool stop_;
};

class EngineWriterListener : public eprosima::fastdds::dds::DataWriterListener
{
public:

    EngineWriterListener()
    {
    }

    ~EngineWriterListener() override
    {
    }

    void on_publication_matched(
            eprosima::fastdds::dds::DataWriter* writer,
            const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

};

class DLReaderListener : public eprosima::fastdds::dds::DataReaderListener
{
public:

    DLReaderListener()
        : samples_(0)
    {
    }

    ~DLReaderListener() override
    {
    }

    void on_data_available(
        eprosima::fastdds::dds::DataReader* reader) override;

    void on_subscription_matched(
        eprosima::fastdds::dds::DataReader* reader,
        const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

private:
    int samples_;
};

class AtomizationReaderListener : public eprosima::fastdds::dds::DataReaderListener
{
public:

    AtomizationReaderListener()
        : samples_(0)
    {
    }

    ~AtomizationReaderListener() override
    {
    }

    void on_data_available(
        eprosima::fastdds::dds::DataReader* reader) override;

    void on_subscription_matched(
        eprosima::fastdds::dds::DataReader* reader,
        const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

private:
    int samples_;
};

#endif /* ENGINEPARTICIPANT_HPP */
