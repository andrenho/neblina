#include "arguments.hh"

#include <getopt.h>
#include <iostream>

#include "util/exceptions/non_recoverable_exception.hh"
using namespace std::string_literals;

Arguments::Arguments(int argc, char* argv[])
    : program_name(argv[0]),
      data_dir(std::string(getenv("HOME")) + "/neblina")
{
    while (true) {
        static option long_options[] = {
            { "help",          no_argument,       nullptr, 'h' },
            { "data-dir",      required_argument, nullptr, 'D' },
            { "verbose",       no_argument,       nullptr, 'v' },

            // used to define services and their characteristics
            { "service",       required_argument, nullptr, 's' },
            { "port",          required_argument, nullptr, 'P' },
            { "open-to-world", no_argument,       nullptr, 'w' },
            { "frequency",     required_argument, nullptr, 'f' },
            { "log-color",     required_argument, nullptr, 'c' },
        };
        int idx;
        int c = getopt_long(argc, argv, "hs:D:P:wf:c:v", long_options, &idx);
        if (c == -1)
            break;

        switch (c) {
            case 'h': print_help(argv[0]); break;
            case 'D': data_dir = optarg; break;
            case 'P': port = (int) strtol(optarg, nullptr, 10); break;
            case 'w': open_to_world = true; break;
            case 'f': frequency = std::chrono::milliseconds(strtoll(optarg, nullptr, 10)); break;
            case 's': service = optarg; break;
            case 'c': logging_color = optarg; break;
            case 'v': verbose = true; break;
            default: throw NonRecoverableException("Unexpected error while parsing arguments");
        }
    }

    if (optind < argc)
        throw NonRecoverableException("Argument "s + argv[optind] + " not supported.");
}

void Arguments::print_help(std::string const& program_name)
{
    std::cout << std::format(R"(Usage: {} [OPTIONS]
    -D, --data-dir [PATH]           Choose data dir path
    -v, --verbose                   Verbose mode (for debugging)
)", program_name);
    exit(0);
}

Arguments const& args(int argc, char* argv[])
{
    static Arguments arguments(argc, argv);
    return arguments;
}

