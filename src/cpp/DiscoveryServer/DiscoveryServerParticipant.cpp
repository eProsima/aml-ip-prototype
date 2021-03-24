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
 * @file DiscoveryServerParticipant.cpp
 *
 */

#include "DiscoveryServerParticipant.hpp"
#include "../types/DLOutput/DLOutputPubSubTypes.h"
#include "../types/utils/utils.hpp"

#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/topic/qos/TopicQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastrtps/xmlparser/XMLProfileManager.h>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>
#include <fastrtps/Domain.h>
#include <fastrtps/participant/Participant.h>

#include <stdlib.h>
#include <atomic>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

DiscoveryServerParticipant::DiscoveryServerParticipant()
    : participant_(nullptr)
{
}

bool DiscoveryServerParticipant::init(
        int tcp_port,
        int udp_port,
        std::string address)
{
    // Load profiles
    eprosima::fastrtps::xmlparser::XMLProfileManager::loadDefaultXMLFile();
    DomainParticipantFactory::get_instance()->load_profiles();

    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos = DomainParticipantFactory::get_instance()->get_default_participant_qos();

    // TODO set Discovery Server from DDS qos
    static_cast<void> (pqos);

    ParticipantAttributes PParam;
    PParam.rtps.builtin.discovery_config.discoveryProtocol = DiscoveryProtocol_t::SERVER;
    PParam.rtps.ReadguidPrefix("01.0f.2d.41.4c.47.45.42.52.41.49.43");
    PParam.rtps.builtin.discovery_config.leaseDuration = c_TimeInfinite;
    PParam.rtps.setName("AML IP Discovery Server");

    // TCP Manual configuration
    // TCP client configuration
    if (tcp_port != -1)
    {
        std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

        //descriptor->wait_for_tcp_negotiation = false;

        descriptor->add_listener_port(tcp_port);
        descriptor->set_WAN_address(address);

        PParam.rtps.userTransports.push_back(descriptor);
    }

    // TCP server configuration
    if (udp_port != -1)
    {
        Locator_t udp_locator(LOCATOR_KIND_UDPv4);
        udp_locator.port = udp_port;
        IPLocator::setIPv4(udp_locator, address);

        PParam.rtps.builtin.metatrafficUnicastLocatorList.push_back(udp_locator);
    }

    participant_ = Domain::createParticipant(PParam);

    if (participant_ == nullptr)
    {
        return false;
    }

    return true;
}

DiscoveryServerParticipant::~DiscoveryServerParticipant()
{
    Domain::removeParticipant(participant_);
}

void DiscoveryServerParticipant::run(
        uint32_t time)
{
    if (time == 0)
    {
        std::cout << "DiscoveryServer Participant " << participant_->getGuid().guidPrefix
            << " running. Please press enter to stop it at any time." << std::endl;
        std::cin.ignore();
    }
    else
    {
        std::cout << "DiscoveryServer Participant " << participant_->getGuid().guidPrefix
            << " running for  " << time << " seconds." << std::endl;
    }
}
