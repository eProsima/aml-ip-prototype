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
 * @file main.cpp
 *
 */

#include <iostream>
#include <string>
#include <vector>

#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>

#include <optionparser.h>

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
    LOGICAL_PORT,
    PHYSICAL_PORT,
    TIME,
    DESCRIPTION
};

/*
 * Usage description
 */
const option::Descriptor usage[] = {
    { UNKNOWN_OPT, 0,"", "",                    Arg::None,
        "Usage: Forward a IGD port to this device"},
    { HELP,    0,"h", "help",                   Arg::None,
        "-h \t--help  \tProduce help message." },
    { LOGICAL_PORT, 0, "l", "logical-port",     Arg::Numeric,
        "-l <num>\t  --logical-port=<num> \tPublic port to open in the router. [Required]"},
    { PHYSICAL_PORT, 0, "p", "physical-port",   Arg::Numeric,
        "-p <num>\t  --physical-port=<num> \tHost port to listen packets from the router. [Required]"},
    { TIME, 0, "t", "time",                     Arg::Numeric,
        "-t <num> \t--time=<num> \tTime in seconds until the port is closed again (Default 60)."},
    { DESCRIPTION, 0, "d", "description",       Arg::String,
        "-d <str> \t--description=<str> \tDescription for the port mapping (Default 'test')."},
    { 0, 0, 0, 0, 0, 0 }
};

int add_port(int logic_port, int physic_port, int time, std::string desc)
{
    struct UPNPDev *upnp_dev = nullptr;
    struct UPNPUrls upnp_urls;
    struct IGDdatas upnp_data;
    int error = 0;
    char aLanAddr[64];

    upnp_dev = upnpDiscover(2000, NULL, NULL, 0, 0, 2, &error);

    if(error != 0)
    {
        std::cout << "error discovering upnp devices: " << strupnperror(error) << std::endl;
        return 1;
    }

    // Retrieve a valid Internet Gateway Device
    int status = UPNP_GetValidIGD(upnp_dev, &upnp_urls, &upnp_data, aLanAddr,
                                sizeof(aLanAddr));

    std::cout << "status: " << status << " ; lan_addr:" << aLanAddr << std::endl;

    if (status > 0)
    {
        std::cout << "found valid IGD: " << upnp_urls.controlURL << std::endl;

        error =
            UPNP_AddPortMapping(upnp_urls.controlURL, upnp_data.first.servicetype,
                                std::to_string(logic_port).c_str(), // external port
                                std::to_string(physic_port).c_str(), // internal port
                                aLanAddr, desc.c_str(), "TCP",
                                0,  // remote host
                                std::to_string(time).c_str() // lease duration, recommended 0 as some NAT
                                    // implementations may not support another value
            );

        if (error)
        {
            std::cout << "failed to map port" << std::endl;
            std::cout << "error: " << strupnperror(error) << std::endl;
        }
        else
        {
            std::cout << "successfully mapped port" << std::endl;
        }
    }
    else
    {
        std::cout << "no valid IGD found" << std::endl;
    }

    FreeUPNPUrls(&upnp_urls);
    freeUPNPDevlist(upnp_dev);

    return 0;
}

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
    int logic_port = -1;
    int physic_port = -1;
    int time = 0;
    std::string desc("test");

    // 2 required arguments
    if (argc > 2)
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

                case LOGICAL_PORT:
                    logic_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case PHYSICAL_PORT:
                    physic_port = std::strtol(opt.arg, nullptr, 10);
                    break;

                case TIME:
                    time = std::strtol(opt.arg, nullptr, 10);
                    break;

                case DESCRIPTION:
                    desc = opt.arg;
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

    // Ports must be specified
    if (logic_port == -1 || physic_port == -1)
    {
        std::cout << "CLI error: Logical and Physical ports must be specified" << std::endl;
        option::printUsage(fwrite, stdout, usage, columns);
        return 1;
    }

    return add_port(logic_port, physic_port, time, desc);
}
