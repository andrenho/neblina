#include "config.hh"

#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

#include "embedded.hh"
#include "default_config.gen.c"

Config::Config(int argc, char* argv[])
{
    std::string config_path = std::string(getenv("HOME")) + "/.config/neblina";
    std::string config_filename = "neblina.conf";

    parse_arguments(argc, argv);

    if (!fs::exists(config_path))
        create_config_file();

    load_config_file();
}

void Config::parse_arguments(int argc, char* argv[])
{
    // TODO - optget
}

void Config::create_config_file()
{
    std::string default_config_file = decode_embedded_text(default_config, sizeof default_config, default_config_uncompressed_sz);
    std::cout << default_config_file;
}

void Config::load_config_file()
{

}
