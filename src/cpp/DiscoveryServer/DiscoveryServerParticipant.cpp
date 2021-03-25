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
#include <sstream>

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

    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod =
            eprosima::fastrtps::Duration_t(2, 0);
    pqos.name("AML IP Discovery Server");

    // Set as a server
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = DiscoveryProtocol::SERVER;

    // Set guid manually
    std::istringstream(SERVER_GUID_PREFIX) >> pqos.wire_protocol().prefix;

    // TCP configuration
    if (tcp_port != -1)
    {
        // Create TCPv4 transport
        std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

        //descriptor->wait_for_tcp_negotiation = false;

        descriptor->add_listener_port(tcp_port);
        descriptor->set_WAN_address(address);

        descriptor->sendBufferSize = 0;
        descriptor->receiveBufferSize = 0;

        pqos.transport().user_transports.push_back(descriptor);

        // Create Locator
        Locator_t tcp_locator(LOCATOR_KIND_TCPv4);

        IPLocator::setIPv4(tcp_locator, address);
        IPLocator::setLogicalPort(tcp_locator, tcp_port);
        IPLocator::setPhysicalPort(tcp_locator, tcp_port);

        pqos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(tcp_locator);
    }

    // UDP configuration
    if (udp_port != -1)
    {
        // There is no need to create descriptor as UDPv4 is already created
        Locator_t udp_locator(LOCATOR_KIND_UDPv4);
        udp_locator.port = udp_port;
        IPLocator::setIPv4(udp_locator, address);

        pqos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(udp_port);
    }

    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

    if (participant_ == nullptr)
    {
        return false;
    }

    return true;
}

DiscoveryServerParticipant::~DiscoveryServerParticipant()
{
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void DiscoveryServerParticipant::run(
        uint32_t time)
{
    if (time == 0)
    {
        std::cout << "DiscoveryServer Participant " << participant_->guid().guidPrefix
            << " running. Please press enter to stop it at any time." << std::endl;
        std::cin.ignore();
    }
    else
    {
        std::cout << "DiscoveryServer Participant " << participant_->guid().guidPrefix
            << " running for  " << time << " seconds." << std::endl;
    }
}
