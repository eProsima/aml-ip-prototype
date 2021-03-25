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

#include "../utils/optionparser.h"

/*
 * Struct to parse the executable arguments
 */
struct Arg: public option::Arg
{
    static void print_error(const char* msg1, const option::Option& opt, const char* msg2)
    {
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
    }

    static option::ArgStatus Unknown(const option::Option& option, bool msg)
    {
        if (msg) print_error("Unknown option '", option, "'\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Required(const option::Option& option, bool msg)
    {
        if (option.arg != 0 && option.arg[0] != 0)
        return option::ARG_OK;

        if (msg) print_error("Option '", option, "' requires an argument\n");
        return option::ARG_ILLEGAL;
    }

    static option::ArgStatus Numeric(const option::Option& option, bool msg)
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

    static option::ArgStatus Float(const option::Option& option, bool msg)
    {
        // char* endptr = 0;
        // if (option.arg != 0 && std::stof(option.arg, &endptr, 10))
        // {
        // }
        // if (endptr != option.arg && *endptr == 0)
        // {
        //     return option::ARG_OK;
        // }

        // if (msg)
        // {
        //     print_error("Option '", option, "' requires a float argument\n");
        // }
        // return option::ARG_ILLEGAL;
        return option::ARG_OK;
    }

    static option::ArgStatus String(const option::Option& option, bool msg)
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
enum  optionIndex {
    UNKNOWN_OPT,
    HELP,
    TCP_PORT,
    UDP_PORT,
    ADDRESS,
    TIME
};

/*
 * Usage description
 */
const option::Descriptor usage[] = {
    { UNKNOWN_OPT, 0,"", "",                Arg::None,
        "Usage: AML IP DiscoveryServer \n" \
        "Set IP address and listening ports.\nTo use WAN connection TCP port must be open from router.\n" \
        "The default FastDDS transports are available.\nGeneral options:" },
    { HELP,    0,"h", "help",               Arg::None,      "  -h \t--help  \tProduce help message." },
    { TCP_PORT, 0, "t", "tcp-port",             Arg::Numeric,
        "-t <num>\t  --tcp-port=<num> \tPort to listen as TCP server (Default 5100)."},
    { UDP_PORT, 0, "u", "udp-port",             Arg::Numeric,
        "-u <num>\t  --udp-port=<num> \tPort to listen in UDP (Default 11811)."},
    { ADDRESS, 0, "a", "address",             Arg::String,
        "-a <address>\t  --address=<address> \t Public IP address to connect from outside the LAN (Default "")."},
    { TIME, 0, "", "time",             Arg::Numeric,
        "--time \tTime in seconds until the server closes, if 0 wait for user input (Default 0)."},
    { 0, 0, 0, 0, 0, 0 }
};

int main(int argc, char** argv)
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
#endif

    // Get executable arguments
    int tcp_port = -1;
    int udp_port = -1;
    uint32_t time;
    std::string address("");

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

                case TCP_PORT:
                    tcp_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case UDP_PORT:
                    udp_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case ADDRESS:
                    address = opt.arg;
                    break;

                case TIME:
                    time = std::strtol(opt.arg, nullptr, 10);
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
    if (address == "")
    {
        std::cout << "CLI error: Public IP address must be specified" << std::endl;
        option::printUsage(fwrite, stdout, usage, columns);
        return 1;
    }

    // Create Participant object and run thread of publishing in loop
    DiscoveryServerParticipant part;
    if (part.init(tcp_port, udp_port, address))
    {
        part.run(time);
    }
    else
    {
        return 2;
    }

    return 0;
}
