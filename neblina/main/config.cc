#include "config.hh"

#include <filesystem>
namespace fs = std::filesystem;

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

}

void Config::create_config_file()
{

}

void Config::load_config_file()
{

}
