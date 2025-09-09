#ifndef CONFIG_HH
#define CONFIG_HH

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include "contrib/simdjson/simdjson.h"

using Config = simdjson::ondemand::document;

class ConfigManager {
public:
    ConfigManager(int argc, char* argv[]);
    ~ConfigManager();

    [[nodiscard]] Config parse_config_file() const;

    std::string service;
    std::string config_filename;
    const std::string program_name;

private:
    void parse_arguments(int argc, char* argv[]);
    void create_config_file(std::string const& filepath);
    static void print_help(std::string const& program_name);

    std::string default_config_path_;
};

#endif //CONFIG_HH
