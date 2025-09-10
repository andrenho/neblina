#include "config_manager.hh"

#include <filesystem>
#include <format>
#include <iostream>
#include <fstream>
#include <string>
namespace fs = std::filesystem;
using namespace std::string_literals;

#include <getopt.h>

#include "../embed/embedded.hh"
#include "default_config.gen.inc"

ConfigManager::ConfigManager(int argc, char* argv[])
    : program_name(argv[0]), default_config_path_(std::string(getenv("HOME")) + "/.config/neblina")
{
    parse_arguments(argc, argv);

    // if config file not indicated, use default, create if not present
    if (config_filename.empty()) {
        config_filename = default_config_path_ + "/neblina.json";
        if (!fs::exists(config_filename))
            create_config_file(config_filename);
    }

    parse_config_file();
}

ConfigManager::~ConfigManager() = default;

void ConfigManager::parse_arguments(int argc, char* argv[])
{
    while (true) {
        static option long_options[] = {
            { "help",     no_argument,       nullptr, 'h' },
            { "config",   required_argument, nullptr, 'C' },
            { "service",  required_argument, nullptr, 's' },
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
                config_filename = optarg;
                break;
            case 's':
                service = optarg;
                break;
        }
    }

    if (optind < argc)
        throw std::runtime_error("Argument "s + argv[optind] + " not supported.");
}

void ConfigManager::create_config_file(std::string const& filepath)
{
    std::string default_config_file = decode_embedded_text(default_config, sizeof default_config, default_config_uncompressed_sz);
    fs::create_directory(default_config_path_);
    std::ofstream out(filepath, std::ios::out | std::ios::trunc);
    if (!out)
        throw std::runtime_error("Could not create default config file");
    out << default_config_file;
}

void ConfigManager::print_help(std::string const& program_name)
{
    std::cout << std::format(R"(Usage: {} [OPTIONS]
    -C, --config [FILE]         Choose config file
)", program_name);
    exit(0);
}

void ConfigManager::parse_config_file()
{
    auto error = parser_.load(config_filename).get(config_);
    if (error)
        throw std::runtime_error("Error parsing config file JSON");
}
