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

#include <string>

#include "../thirdparty/optionparser.h"

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

};

// TODO is possible to open 2 transports in same port, so tcp and udp could be set as same port for future versions
/*
 * Option arguments available
 */
enum  optionIndex
{
    UNKNOWN_OPT,
    HELP,
    LISTENING_ADDRESS,
    LISTENING_PORT,
    LISTENING_ID,
    CONNECTION_ADDRESS,
    CONNECTION_PORT,
    CONNECTION_ID,
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
      "The default FastDDS transports are available.\nGeneral options:" },
    { HELP,    0, "h", "help",                   Arg::None,      "  -h \t--help  \tProduce help message." },
    { LISTENING_ADDRESS, 0, "a", "listening-address",               Arg::String,
      "  -a <address> \t--listening-address=<address> \t Public IP address to connect from outside the LAN (Default '127.0.0.1')."},
    { LISTENING_PORT, 0, "p", "listening-port",                 Arg::Numeric,
      "  -p <num> \t--listening-port=<num> \tPort to listen as TCP server (Default 5100)."},
    { LISTENING_ID, 0, "i", "listening-id",                      Arg::Numeric,
      "  -i <num>\t--listening-id=<num> \tId of the Discovery Server to create the GUID (Default 0)."},
    { CONNECTION_ADDRESS, 0, "", "connection-address",               Arg::String,
      "  --connection-address=<address> \t IP address to connect with other Discovery Server. Set to remotely connection"},
    { CONNECTION_PORT, 0, "", "connection-port",                 Arg::Numeric,
      "  --connection-port=<num> \tPort to connect with another Discovery Server (Default 5100)."},
    { CONNECTION_ID, 0, "", "connection-id",                      Arg::Numeric,
      "  --connection-id=<num> \tId of the Discovery Server to connet remotely (set the GUID) (Default 0)."},
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
    columns = getenv("COLUMNS") ? atoi(getenv("COLUMNS")) : 80;
#endif // if defined(_WIN32)

    // Get executable arguments
    uint32_t time = 0;
    int listening_port = 5100;
    std::string listening_address("127.0.0.1");
    int listening_id = 0;
    int connection_port = 5100;
    std::string connection_address("");
    int connection_id = 0;
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

                case LISTENING_PORT:
                    listening_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case LISTENING_ADDRESS:
                    listening_address = opt.arg;
                    break;

                case LISTENING_ID:
                    listening_id = std::strtol(opt.arg, nullptr, 10);
                    break;

                case CONNECTION_PORT:
                    connection_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case CONNECTION_ADDRESS:
                    connection_address = opt.arg;
                    break;

                case CONNECTION_ID:
                    connection_id = std::strtol(opt.arg, nullptr, 10);
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

    // Public Address must be specified
    if (listening_address == "")
    {
        std::cout << "CLI error: Public IP address must be specified" << std::endl;
        option::printUsage(fwrite, stdout, usage, columns);
        return 1;
    }

    // Create Participant object and run thread of publishing in loop
    DiscoveryServerParticipant part;
    if (part.init(
                listening_port,
                listening_address,
                listening_id,
                connection_port,
                connection_address,
                connection_id,
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
