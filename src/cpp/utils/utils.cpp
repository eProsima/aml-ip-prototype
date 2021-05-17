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
 * @file utils.cpp
 *
 */

#include <sstream>
#include <iostream>

#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastrtps/xmlparser/XMLProfileManager.h>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>

#include "utils.hpp"
#include "../types/types.hpp"

using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace eprosima::fastrtps::rtps;

AML_IP_Atomization generate_random_atomization_data(
        uint32_t data_size)
{
    AML_IP_Atomization data;
    std::vector<AML_IP_Atom> atoms;

    int atoms_number = (rand() % data_size) + 1;
    for (int i = 0; i < atoms_number; ++i)
    {
        AML_IP_Atom atom_data;

        // The size of the atoms are always the same
        std::vector<uint32_t> ucs(data_size);
        for (unsigned int j = 0; j < data_size; ++j)
        {
            ucs[j] = rand();
        }

        atom_data.ucs(ucs);

        atoms.push_back(atom_data);
    }
    data.atoms(atoms);

    return data;
}

AML_IP_DLOutput generate_random_dloutput_data(
        uint32_t data_size)
{
    AML_IP_DLOutput data;
    std::vector<AML_IP_Relation> relations;

    int relation_number = (rand() % data_size) + 1;
    for (int i = 0; i < relation_number; ++i)
    {
        AML_IP_Relation relation_data;

        int l_relation_number = (rand() % data_size) + 1;
        int h_relation_number = (rand() % data_size) + 1;

        std::vector<uint32_t> l_relations(l_relation_number);
        for (int j = 0; j < l_relation_number; ++j)
        {
            l_relations[j] = rand() % data_size;
        }

        std::vector<uint32_t> h_relations(h_relation_number);
        for (int j = 0; j < h_relation_number; ++j)
        {
            h_relations[j] = rand() % data_size;
        }

        bool type_of_relation = (0 == rand() % 2);

        relation_data.l(l_relations);
        relation_data.h(h_relations);
        relation_data.typeOfRelation(type_of_relation);

        relations.push_back(relation_data);
    }
    data.relations(relations);

    return data;
}

std::ostream& operator <<(
        std::ostream& output,
        const AML_IP_Atomization& data)
{
    output << "<Number of atoms: " << data.atoms().size() << ">";
    return output;
}

std::ostream& operator <<(
        std::ostream& output,
        const AML_IP_DLOutput& data)
{
    output << "<Number of relations: " << data.relations().size() << ">";
    return output;
}

std::vector<std::pair<std::string, uint16_t>> split_locator(std::string addresses, std::string value_delimiter, std::string address_delimiter)
{
    std::vector<std::pair<std::string, uint16_t>> result;

    size_t pos_ini = 0;
    size_t pos = 0;
    std::string token;

    if (addresses == "")
    {
        return result;
    }

    if (addresses.find(address_delimiter) == std::string::npos)
    {
        // get address ip
        std::string ip = addresses.substr(0, addresses.find(value_delimiter));

        // get port
        uint16_t port = std::stol(addresses.substr(ip.length()+1));

        result.push_back(std::make_pair(ip, port));

        return result;
    }

    do
    {
        pos = addresses.find(address_delimiter, pos_ini);

        token = addresses.substr(pos_ini, pos - pos_ini);

        // get address ip
        std::string ip = addresses.substr(pos_ini, token.find(value_delimiter));

        // get port
        uint16_t port = std::stol(token.substr(ip.length()+1));

        result.push_back(std::make_pair(ip, port));
        pos_ini = pos + 1;

    }while (pos != std::string::npos);

    return result;
}

std::vector<std::tuple<std::string, uint16_t, uint16_t>> split_ds_locator(std::string addresses, std::string value_delimiter, std::string address_delimiter)
{
    std::vector<std::tuple<std::string, uint16_t, uint16_t>> result;

    size_t pos_ini = 0;
    size_t pos = 0;
    std::string token;

    size_t first_delimiter_pos = 0;
    size_t second_delimiter_pos = 0;

    if (addresses == "")
    {
        return result;
    }

    if (addresses.find(address_delimiter) == std::string::npos)
    {
        first_delimiter_pos = addresses.find(value_delimiter);
        second_delimiter_pos = addresses.find(value_delimiter, first_delimiter_pos+1);

        // get address ip
        std::string ip = addresses.substr(0, first_delimiter_pos);

        // get port
        uint16_t port = std::stol(addresses.substr(first_delimiter_pos+1, second_delimiter_pos));

        // get id
        uint16_t id = std::stol(addresses.substr(second_delimiter_pos+1));

        result.push_back(std::make_tuple(ip, port, id));

        return result;
    }

    do
    {

        pos = addresses.find(address_delimiter, pos_ini);

        token = addresses.substr(pos_ini, pos - pos_ini);

        first_delimiter_pos = token.find(value_delimiter);
        second_delimiter_pos = token.find(value_delimiter, first_delimiter_pos+1);

        // get address ip
        std::string ip = token.substr(0, first_delimiter_pos);

        // get port
        uint16_t port = std::stol(token.substr(first_delimiter_pos+1, second_delimiter_pos));

        // get id
        uint16_t id = std::stol(token.substr(second_delimiter_pos+1));

        result.push_back(std::make_tuple(ip, port, id));
        pos_ini = pos + 1;

    } while (pos != std::string::npos);

    return result;
}

std::stringstream print_locator(std::string addresses, std::string value_delimiter, std::string address_delimiter)
{
    std::stringstream result;
    for (auto locator : split_locator(addresses, value_delimiter, address_delimiter))
    {
        result << "IP: " << locator.first << "  port: " << locator.second << std::endl;
    }
    return result;
}

std::stringstream print_ds_locator(std::string addresses, std::string value_delimiter, std::string address_delimiter)
{
    std::stringstream result;
    for (auto locator : split_ds_locator(addresses, value_delimiter, address_delimiter))
    {
        result << "IP: " << std::get<0>(locator) << "  port: " << std::get<1>(locator)
            << "  id: " << std::get<2>(locator) << std::endl;
    }
    return result;
}

eprosima::fastdds::dds::DomainParticipantQos get_node_qos(
        std::string name,
        std::string connection_address,
        std::string listening_address)
{
    // Load profiles
    eprosima::fastrtps::xmlparser::XMLProfileManager::loadDefaultXMLFile();
    DomainParticipantFactory::get_instance()->load_profiles();

    //CREATE THE PARTICIPANT
    DomainParticipantQos pqos = DomainParticipantFactory::get_instance()->get_default_participant_qos();

    pqos.wire_protocol().builtin.discovery_config.leaseDuration = eprosima::fastrtps::c_TimeInfinite;
    pqos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod =
            eprosima::fastrtps::Duration_t(2, 0);
    pqos.name(name);

    // Set as a client
    pqos.wire_protocol().builtin.discovery_config.discoveryProtocol = DiscoveryProtocol::CLIENT;

    // This variable is used to set a TCP descriptor in case there are not listening addresses
    bool tcp_descriptor_set = false;

    // Listening configuration
    for (auto address : split_locator(listening_address))
    {
        tcp_descriptor_set = true;

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

    // When there are not listening addresses there still could bec executed as a TCP Server
    if (!tcp_descriptor_set)
    {
        // TCP client configuration
        std::shared_ptr<TCPv4TransportDescriptor> descriptor = std::make_shared<TCPv4TransportDescriptor>();

        descriptor->sendBufferSize = 0;
        descriptor->receiveBufferSize = 0;

        pqos.transport().user_transports.push_back(descriptor);
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

    return pqos;
}
