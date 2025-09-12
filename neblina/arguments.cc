#include "arguments.hh"

#include <getopt.h>
#include <iostream>
using namespace std::string_literals;

Arguments::Arguments(int argc, char* argv[])
    : program_name(argv[0]),
      data_dir(std::string(getenv("HOME")) + "/neblina")
{
    while (true) {
        static option long_options[] = {
            { "help",          no_argument,       nullptr, 'h' },
            { "data-dir",      required_argument, nullptr, 'D' },
            { "service",       required_argument, nullptr, 's' },
        };
        int idx;
        int c = getopt_long(argc, argv, "hs:D:", long_options, &idx);
        if (c == -1)
            break;

        switch (c) {
            case 'h':
                print_help(argv[0]);
                break;
            case 'D':
                data_dir = optarg;
                break;
            case 's':
                service = optarg;
                break;
        }
    }

    if (optind < argc)
        throw std::runtime_error("Argument "s + argv[optind] + " not supported.");
}

void Arguments::print_help(std::string const& program_name)
{
    std::cout << std::format(R"(Usage: {} [OPTIONS]
    -D, --data-dir [PATH]         Choose data dir path
)", program_name);
    exit(0);
}

Arguments const& args(int argc, char* argv[])
{
    static Arguments arguments(argc, argv);
    return arguments;
}

