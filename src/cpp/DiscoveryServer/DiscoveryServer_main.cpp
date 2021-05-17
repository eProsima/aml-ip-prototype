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
 * @file DiscoveryServer_main.cpp
 *
 */

#include "DiscoveryServerParticipant.hpp"

#include <iostream>
#include <regex>
#include <string>

#include "../utils/utils.hpp"
#include "../thirdparty/optionparser.h"

static const std::regex ipv4_port_id("(^(((((([0-9]{1,3}\\.){1,3})([0-9]{1,3})),([0-9]{1,5}),([0-9]+));)*"
                              "((((([0-9]{1,3}\\.){1,3})([0-9]{1,3})),([0-9]{1,5}),([0-9]+))?))$)");
static const std::regex ipv4_port("(^(((((([0-9]{1,3}\\.){1,3})([0-9]{1,3})),([0-9]{1,5}));)*"
                              "(((([0-9]{1,3}\\.){1,3})([0-9]{1,3})),([0-9]{1,5})))$)");

/*
 * Struct to parse the executable arguments
 */
struct Arg : public option::Arg
{
    static void print_error(
            const char* msg1,
            const option::Option& opt,
            const char* msg2)
    {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Unknown(
            const option::Option& option,
            bool msg)
    {
        if (msg)
        {
            print_error("Unknown option '", option, "'\n");
        }
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Required(
            const option::Option& option,
            bool msg)
    {
        if (option.arg != 0 && option.arg[0] != 0)
        {
            return option::ARG_OK;
        }

        if (msg)
        {
            print_error("Option '", option, "' requires an argument\n");
        }
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Numeric(
            const option::Option& option,
            bool msg)
    {
        char* endptr = 0;
        if (option.arg != 0 && std::strtol(option.arg, &endptr, 10))
        {
        }
        if (endptr != option.arg && *endptr == 0)
        {
            return option::ARG_OK;
        }

        if (msg)
        {
            print_error("Option '", option, "' requires a numeric argument\n");
        }
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Float(
            const option::Option& option,
            bool msg)
    {
        char* endptr = 0;
        if (option.arg != 0 && std::strtof(option.arg, &endptr))
        {
        }
        if (endptr != option.arg && *endptr == 0)
        {
            return option::ARG_OK;
        }

        if (msg)
        {
            print_error("Option '", option, "' requires a float argument\n");
        }
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus String(
            const option::Option& option,
            bool msg)
    {
        if (option.arg != 0)
        {
            return option::ARG_OK;
        }
        if (msg)
        {
            print_error("Option '", option, "' requires a numeric argument\n");
        }
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Locator(
            const option::Option& option,
            bool msg)
    {
        if (option.arg != 0)
        {
            // we must check if its a correct ip address plus port number
            if (std::regex_match(option.arg, ipv4_port))
            {
                return option::ARG_OK;
            }
        }
        if (msg)
        {
            print_error("Option '", option, "' requires an ip,port[;ip,port[;...]] argument\n");
        }
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus DSLocator(
            const option::Option& option,
            bool msg)
    {
        if (option.arg != 0)
        {
            // we must check if its a correct ip address plus port number
            if (std::regex_match(option.arg, ipv4_port_id))
            {
                return option::ARG_OK;
            }
        }
        if (msg)
        {
            print_error("Option '", option, "' requires an ip,port,id[;ip,port,id[;...]] argument\n");
        }
        return option::ARG_ILLEGAL;
    }

};

// TODO is possible to open 2 transports in same port, so tcp and udp could be set as same port for future versions
/*
 * Option arguments available
 */
enum  optionIndex
{
    UNKNOWN_OPT,
    HELP,
    LISTENING_ADDRESSES,
    CONNECTION_ADDRESSES,
    ID,
    TIME,
    BACKUP
};

/*
 * Usage description
 */
const option::Descriptor usage[] = {
    { UNKNOWN_OPT, 0, "", "",                    Arg::None,
      "Usage: AML IP DiscoveryServer \n" \
      "Set IP address and listening ports.\nTo use WAN connection TCP port must be open from router.\n" \
      "Listening Addresses are on format <IPaddress,port[;IPaddress,port[;...]]> ; string of pair <ip,port> " \
      "separated with ';'. Valid address values are '127.0.0.1,1;255.255.255.255,35000'\n"
      "Connection Addresses are on format <IPaddress,port,id[;IPaddress,port,id[;...]]> ; string of tuples " \
      "<ip,port,id> separated with ';'. Valid address values are '127.0.0.1,1,0;255.255.255.255,35000,1000'\n"
      "The default FastDDS transports are available.\nGeneral options:" },

    { HELP,    0, "h", "help",                   Arg::None,      "  -h \t--help  \tProduce help message." },

    { LISTENING_ADDRESSES, 0, "l", "listening-addresses",               Arg::Locator,
      "  -l <adresses>\t--listening-addresses=<addresses> \t IP addresses where server will listen " \
      "for external petitions and will receive replies from other servers (Default '127.0.0.1,5000')."},

    { CONNECTION_ADDRESSES, 0, "c", "connection-addresses",               Arg::DSLocator,
      "  -c <addresses>\t --connection-addresses=<addresses> \t IP addresses of other servers and their ids that this "
      " server will try to connect with (Default '')."},

    { ID, 0, "i", "id",               Arg::Numeric,
      "  -i <uint>\t --id=<uint> \t Id to the Discovery Server created. Use this id with the server address to " \
      "connect with it as this server client. This id will set the GUID of the Discovery Server (Default 0)."},

    { TIME, 0, "t", "time",                      Arg::Numeric,
      "  -t <num>\t--time=<num> \tTime in seconds until the server closes, if 0 wait for user input (Default 0)."},

    { BACKUP, 0, "b", "backup",              Arg::None,
      "  -b \t--backup \tSet Discovery Server as Backup. Use only for debug purpose and erase old db before execute."},

    { 0, 0, 0, 0, 0, 0 }
};

int main(
        int argc,
        char** argv)
{
    // Variable to pretty print usage help
    int columns;
#if defined(_WIN32)
    char* buf = nullptr;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, "COLUMNS") == 0 && buf != nullptr)
    {
        columns = strtol(buf, nullptr, 10);
        free(buf);
    }
    else
    {
        columns = 80;
    }
#else
    columns = getenv("COLUMNS") ? atoi(getenv("COLUMNS")) : 180;
#endif // if defined(_WIN32)

    // Get executable arguments
    uint32_t time = 0;
    std::string listening_addresses("127.0.0.1,5000");
    uint16_t id = 0;
    std::string connection_addresses("");
    bool backup = false;

    // No required arguments
    if (argc > 0)
    {

        argc -= (argc > 0); // reduce arg count of program name if present
        argv += (argc > 0); // skip program name argv[0] if present

        option::Stats stats(usage, argc, argv);
        std::vector<option::Option> options(stats.options_max);
        std::vector<option::Option> buffer(stats.buffer_max);
        option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

        if (parse.error())
        {
            return 1;
        }

        if (options[HELP])
        {
            option::printUsage(fwrite, stdout, usage, columns);
            return 0;
        }

        for (int i = 0; i < parse.optionsCount(); ++i)
        {
            option::Option& opt = buffer[i];
            switch (opt.index())
            {
                case HELP:
                    option::printUsage(fwrite, stdout, usage, columns);
                    return 0;
                    break;

                case LISTENING_ADDRESSES:
                    // Must contain at least one direction
                    listening_addresses = opt.arg;
                    break;

                case CONNECTION_ADDRESSES:
                    // There is no restriction with the connection
                    connection_addresses = opt.arg;
                    break;

                case ID:
                    id = std::strtol(opt.arg, nullptr, 10);
                    break;

                case TIME:
                    time = std::strtol(opt.arg, nullptr, 10);
                    break;

                case BACKUP:
                    backup = true;
                    break;

                case UNKNOWN_OPT:
                    option::printUsage(fwrite, stdout, usage, columns);
                    return 1;
                    break;
            }
        }
    }
    else
    {
        option::printUsage(fwrite, stdout, usage, columns);
        return 1;
    }

    // for (auto x : split_locator("127.0.0.1,1"))
    // {
    //     std::cout << std::get<0>(x) << " " << std::get<1>(x) << std::endl;
    // }
    // std::cout << std::endl;
    // for (auto x : split_locator("127.0.0.1,1;192.168.0.1,5550;999.999.999.999,46000"))
    // {
    //     std::cout << std::get<0>(x) << " " << std::get<1>(x) << std::endl;
    // }

    // std::cout << std::endl;

    // for (auto x : split_ds_locator("127.0.0.1,1,1"))
    // {
    //     std::cout << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x) << std::endl;
    // }
    // std::cout << std::endl;
    // for (auto x : split_ds_locator("127.0.0.1,1;192.168.0.1,5550,0;999.999.999.999,46000,45000"))
    // {
    //     std::cout << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x) << std::endl;
    // }

    // Create Participant object and run thread of publishing in loop
    DiscoveryServerParticipant part;
    if (part.init(
                listening_addresses,
                connection_addresses,
                id,
                backup))
    {
        part.run(time);
    }
    else
    {
        return 2;
    }

    return 0;
}
