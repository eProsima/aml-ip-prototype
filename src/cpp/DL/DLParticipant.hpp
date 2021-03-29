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
 * @file DLParticipant.hpp
 *
 */

#ifndef DLPARTICIPANT_HPP
#define DLPARTICIPANT_HPP

#include "../types/DLOutput/DLOutput.h"
#include "../types/types.hpp"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <atomic>

class DLListener;

class DLParticipant
{

public:

    DLParticipant();

    virtual ~DLParticipant();

    //!Initialize
    bool init(
        int domain,
        int connection_port,
        std::string connection_address,
        int listening_port,
        std::string listening_address);

    //!Publish a sample
    bool publish(AML_IP_DLOutput data);

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

    eprosima::fastdds::dds::Topic* topic_;

    eprosima::fastdds::dds::DataWriter* writer_;

    DLListener* listener_;

    eprosima::fastdds::dds::TypeSupport type_;

    std::atomic_bool stop_;

    uint_fast32_t data_size;
};

class DLListener : public eprosima::fastdds::dds::DataWriterListener
{
public:

    DLListener()
    {
    }

    ~DLListener() override
    {
    }

    void on_publication_matched(
            eprosima::fastdds::dds::DataWriter* writer,
            const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;

};

#endif /* DLPARTICIPANT_HPP */
