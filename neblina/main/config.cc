#include "config.hh"

#include <filesystem>
#include <iostream>
#include <fstream>
namespace fs = std::filesystem;

#include "embedded.hh"
#include "default_config.gen.c"

Config::Config(int argc, char* argv[])
{
    std::string config_filename = default_config_file_path() + "/neblina.conf";

    parse_arguments(argc, argv);

    if (!fs::exists(config_filename))
        create_config_file(config_filename);

    load_config_file();
}

void Config::parse_arguments(int argc, char* argv[])
{
    // TODO - optget
}

void Config::create_config_file(std::string const& filepath)
{
    std::string default_config_file = decode_embedded_text(default_config, sizeof default_config, default_config_uncompressed_sz);
    fs::create_directory(default_config_file_path());
    std::ofstream out(filepath, std::ios::out | std::ios::trunc);
    if (!out)
        throw std::runtime_error("Could not create default config file");
    out << default_config_file;
}

void Config::load_config_file()
{

}

std::string Config::default_config_file_path()
{
    return std::string(getenv("HOME")) + "/.config/neblina";
}
