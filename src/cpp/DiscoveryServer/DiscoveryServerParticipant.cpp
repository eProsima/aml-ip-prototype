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

#include <atomic>
#include <stdlib.h>
#include <sstream>
#include <tuple>

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

DiscoveryServerParticipant::DiscoveryServerParticipant()
    : participant_(nullptr)
    , listening_address_("")
    , connection_address_("")
    , listener_(nullptr)
    , server_guid_(GuidPrefix_t::unknown())
{
}

bool DiscoveryServerParticipant::init(
        std::string listening_address,
        std::string connection_address,
        uint16_t id,
        bool backup)
{
    // Store info for printing
    listening_address_ = listening_address;
    connection_address_ = connection_address;
    id_ = id;

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
    if (backup)
    {
        pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = DiscoveryProtocol::BACKUP;
    }
    else
    {
        pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = DiscoveryProtocol::SERVER;
    }

    // Set guid manually depending on the id
    pqos.wire_protocol().prefix = guid_server(id);
    server_guid_ = pqos.wire_protocol().prefix;

    // Listening configuration
    for (auto address : split_locator(listening_address))
    {
        // Create TCPv4 transport
        std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

        descriptor->add_listener_port(std::get<1>(address));
        descriptor->set_WAN_address(std::get<0>(address));

        descriptor->sendBufferSize = 0;
        descriptor->receiveBufferSize = 0;

        pqos.transport().user_transports.push_back(descriptor);

        // Create Locator
        Locator_t tcp_locator;
        tcp_locator.kind = LOCATOR_KIND_TCPv4;

        IPLocator::setIPv4(tcp_locator, std::get<0>(address));
        IPLocator::setWan(tcp_locator, std::get<0>(address));
        IPLocator::setLogicalPort(tcp_locator, std::get<1>(address));
        IPLocator::setPhysicalPort(tcp_locator, std::get<1>(address));

        pqos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(tcp_locator);
    }

    // Add every connection address as server
    for (auto address : split_ds_locator(connection_address))
    {
        // Set Server guid manually
        RemoteServerAttributes server_attr;
        server_attr.guidPrefix = guid_server(std::get<2>(address));

        // Discovery server locator configuration TCP
        Locator_t tcp_locator;
        tcp_locator.kind = LOCATOR_KIND_TCPv4;
        IPLocator::setIPv4(tcp_locator, std::get<0>(address));
        IPLocator::setLogicalPort(tcp_locator, std::get<1>(address));
        IPLocator::setPhysicalPort(tcp_locator, std::get<1>(address));
        server_attr.metatrafficUnicastLocatorList.push_back(tcp_locator);

        pqos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(server_attr);
    }

    listener_ = new DiscoveryServerListener();

    participant_ = DomainParticipantFactory::get_instance()->create_participant(DEFAULT_DOMAIN, pqos, listener_);

    if (participant_ == nullptr)
    {
        return false;
    }

    return true;
}

DiscoveryServerParticipant::~DiscoveryServerParticipant()
{
    if (listener_)
    {
        delete listener_;
    }

    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

void DiscoveryServerParticipant::run(
        uint32_t time)
{
    if (time == 0)
    {
        std::cout << "DiscoveryServer Participant " << participant_->guid().guidPrefix << std::endl
                  << " with id " << id_ << " and GUID " << server_guid_ << std::endl
                  << " listening in addresses: " << std::endl
                  << print_locator(listening_address_).get() // << std::endl // added in print_locator
                  << " connecting with servers in addresses " << std::endl
                  << print_ds_locator(connection_address_).get() // << std::endl // added in print_locator
                  << "Please press enter to stop it at any time." << std::endl;
        std::cin.ignore();
    }
    else
    {
        std::cout << "DiscoveryServer Participant " << participant_->guid().guidPrefix << std::endl
                  << " with id " << id_ << " and GUID " << server_guid_ << std::endl
                  << " listening in addresses: " << std::endl
                  << print_locator(listening_address_).get() // << std::endl // added in print_locator
                  << " connecting with servers in addresses " << std::endl
                  << print_ds_locator(connection_address_).get() // << std::endl // added in print_locator
                  << " for " << time << " seconds." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(time));
    }
}

void DiscoveryServerListener::on_participant_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::ParticipantDiscoveryInfo&& info)
{
    if (info.status == ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT)
    {
        std::cout << "DiscoveryServer Participant " << participant->guid().guidPrefix
                  << " discovered participant " << info.info.m_guid.guidPrefix << std::endl;
    }
    else if (info.status == ParticipantDiscoveryInfo::DROPPED_PARTICIPANT)
    {
        std::cout << "DiscoveryServer Participant " << participant->guid().guidPrefix
                  << " dropped participant " << info.info.m_guid.guidPrefix << std::endl;
    }
    else if (info.status == ParticipantDiscoveryInfo::REMOVED_PARTICIPANT)
    {
        std::cout << "DiscoveryServer Participant " << participant->guid().guidPrefix
                  << " removed participant " << info.info.m_guid.guidPrefix << std::endl;
    }
}

void DiscoveryServerListener::on_subscriber_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::ReaderDiscoveryInfo&& info)
{
    if (info.status == ReaderDiscoveryInfo::DISCOVERED_READER)
    {
        std::cout << "DiscoveryServer Participant " << participant->guid().guidPrefix
                  << " discovered subscriber " << info.info.guid().guidPrefix << std::endl;
    }
    else if (info.status == ReaderDiscoveryInfo::REMOVED_READER)
    {
        std::cout << "DiscoveryServer Participant " << participant->guid().guidPrefix
                  << " remove subscriber " << info.info.guid().guidPrefix << std::endl;
    }
}

void DiscoveryServerListener::on_publisher_discovery(
        eprosima::fastdds::dds::DomainParticipant* participant,
        eprosima::fastrtps::rtps::WriterDiscoveryInfo&& info)
{
    if (info.status == WriterDiscoveryInfo::DISCOVERED_WRITER)
    {
        std::cout << "DiscoveryServer Participant " << participant->guid().guidPrefix
                  << " discovered publisher " << info.info.guid().guidPrefix << std::endl;
    }
    else if (info.status == WriterDiscoveryInfo::REMOVED_WRITER)
    {
        std::cout << "DiscoveryServer Participant " << participant->guid().guidPrefix
                  << " removed publisher " << info.info.guid().guidPrefix << std::endl;
    }
}
