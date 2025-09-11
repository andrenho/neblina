#include "arguments.hh"

#include <getopt.h>
#include <format>
#include <iostream>
using namespace std::string_literals;

void Arguments::print_help(std::string const& program_name)
{
    std::cout << std::format(R"(Usage: {} [OPTIONS]
    -C, --config-path [FILE]         Choose config files path
)", program_name);
    exit(0);
}

Arguments::Arguments(int argc, char* argv[])
    : program_name(argv[0])
{
    config_file_path = std::string(getenv("HOME")) + "/.config/neblina";

    while (true) {
        static option long_options[] = {
            { "help",          no_argument,       nullptr, 'h' },
            { "config-path",   required_argument, nullptr, 'C' },
            { "service",       required_argument, nullptr, 's' },
        };
        int idx;
        int c = getopt_long(argc, argv, "hs:C:", long_options, &idx);
        if (c == -1)
            break;

        switch (c) {
            case 'h':
                print_help(argv[0]);
                break;
            case 'C':
                config_file_path = optarg;
                break;
            case 's':
                service = optarg;
                break;
        }
    }

    if (optind < argc)
        throw std::runtime_error("Argument "s + argv[optind] + " not supported.");
}