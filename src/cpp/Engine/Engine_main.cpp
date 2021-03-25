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
 * @file DL_main.cpp
 *
 */

#include "EngineParticipant.hpp"

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

/*
 * Option arguments available
 */
enum  optionIndex {
    UNKNOWN_OPT,
    HELP,
    PERIOD,
    SAMPLES,
    DOMAIN,
    DATA_SIZE,
    CONNECTION_PORT,
    CONNECTION_ADDRESS,
    LISTENING_PORT,
    LISTENING_ADDRESS
};

/*
 * Usage description
 */
const option::Descriptor usage[] = {
    { UNKNOWN_OPT, 0,"", "",                Arg::None,
        "Usage: AML IP Engine \n" \
        "Connecting port and address must specify where the Discovery Server is listening.\n" \
        "Listening port and address are not required, but without them this client could only work as a TCP client.\n" \
        "General options:" },
    { HELP,    0,"h", "help",               Arg::None,      "  -h \t--help  \tProduce help message." },
    { PERIOD, 0, "p", "period",             Arg::Float,
        "  -p <float> \t--period=<float> \tPeriod to send new random data (Default: 2)." },
    { SAMPLES, 0, "s", "samples",             Arg::Numeric,
        "  -s <num> \t--samples=<num> \tNumber of samples to send (Default: 10)." \
        " With samples=0 it keept sending till enter is pressed" },
    { DOMAIN, 0, "d", "domain",             Arg::Numeric,
        "  -d <num> \t--domain=<num> \tNumber of domain (Default: 11)."},
    { DATA_SIZE, 0, "l", "size",             Arg::Numeric,
        "  -l <num> \t--size=<num> \tMax number of relations in data to send(Default: 5)." \
        " This value also works as seed for randome generation."},
    { CONNECTION_PORT, 0, "", "connection-port",             Arg::Numeric,
        "  --connection-port=<num> \tPort where the TCP server is listening (Default: 5100)."},
    { CONNECTION_ADDRESS, 0, "", "connection-address",             Arg::String,
        "  --connection-address=<address> \tIP address where the TCP server is listening (Default: '')."},
    { LISTENING_PORT, 0, "", "listening-port",             Arg::Numeric,
        "  --listening-port=<num> \tPort to listen as TCP server (Default: -1)."},
    { LISTENING_ADDRESS, 0, "", "listening-address",             Arg::String,
        "  --listening-address=<address> \tIP address to listen as TCP server (Default: '')."},
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
        columns = std::strtol(buf, nullptr, 10);
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
    float period = 2;
    int samples = 10;
    int domain = 11;
    uint32_t data_size = 5;
    int connection_port = 5100;
    std::string connection_address("");
    int listening_port = -1;
    std::string listening_address("");

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

                case PERIOD:
                    period = std::stof(opt.arg);
                    break;

                case SAMPLES:
                    samples = std::strtol(opt.arg, nullptr, 10);
                    break;

                case DOMAIN:
                    domain = std::strtol(opt.arg, nullptr, 10);
                    break;

                case DATA_SIZE:
                    data_size = std::strtol(opt.arg, nullptr, 10);
                    break;

                case CONNECTION_PORT:
                    connection_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case CONNECTION_ADDRESS:
                    connection_address = opt.arg;
                    break;

                case LISTENING_PORT:
                    listening_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case LISTENING_ADDRESS:
                    listening_address = opt.arg;
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
    if (connection_address == "")
    {
        std::cout << "CLI error: Discovery Server IP address must be specified" << std::endl;
        option::printUsage(fwrite, stdout, usage, columns);
        return 1;
    }

    // Initialize random seed
    srand(data_size);

    // Create Participant object and run thread of publishing in loop
    EngineParticipant part;
    if (part.init(domain, connection_port, connection_address, listening_port, listening_address))
    {
        part.run(samples, period, data_size);
    }
    else
    {
        return 2;
    }

    return 0;
}
