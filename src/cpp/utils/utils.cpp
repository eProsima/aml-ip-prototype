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

#include "utils.hpp"

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

    std::cout << addresses << std::endl;

    if (addresses.find(address_delimiter) == std::string::npos)
    {
        first_delimiter_pos = addresses.find(value_delimiter);
        second_delimiter_pos = addresses.find(value_delimiter, first_delimiter_pos+1);

        std::cout << first_delimiter_pos << " and " << second_delimiter_pos << std::endl;

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

    }while (pos != std::string::npos);

    return result;
}
