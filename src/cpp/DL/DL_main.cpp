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

#include "DLParticipant.hpp"

#include <string>

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
        if (option.arg != 0 && strtol(option.arg, &endptr, 10))
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

        static option::ArgStatus __HAVE_FLOAT128X(const option::Option& option, bool msg)
    {
        char* endptr = 0;
        if (option.arg != 0 && stof(option.arg, &endptr, 10))
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
    DOMAIN
};

/*
 * Usage description
 */
const option::Descriptor usage[] = {
    { UNKNOWN_OPT, 0,"", "",                Arg::None,
        "Usage: HelloWorldExampleTCP <publisher|subscriber>\n\nGeneral options:" },
    { HELP,    0,"h", "help",               Arg::None,      "  -h \t--help  \tProduce help message." },
    { PERIOD, 0, "p", "period",             Arg::Float,
        "  -p <float> \t--period=<float> \tPeriod to send new random data (Default: 2)." },
    { SAMPLES, 0, "s", "samples",             Arg::Numeric,
        "  -s <num> \t--samples=<num> \tNumber of samples to send (Default: 10)." \
        " With samples=0 it keept sending till enter is pressed" },
    { DOMAIN, 0, "d", "domain",             Arg::Numeric,
        "  -d <num> \t--domain=<num> \tNumber of domain (Default: 11)."},
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
    float period = 2;
    int samples = 10;
    int domain = 11;

    if (argc > 1)
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
                    period = stof(opt.arg, nullptr, 10);
                    break;

                case SAMPLES:
                    samples = strtol(opt.arg, nullptr, 10);
                    break;

                case DOMAIN:
                    domain = strtol(opt.arg, nullptr, 10);
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

    DLParticipant part;
    if (part.init(domain))
    {
        part.run(samples, period);
    }
    else
    {
        return 2;
    }

    return 0;
}