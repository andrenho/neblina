#ifndef CONFIG_HH
#define CONFIG_HH

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include "contrib/simdjson/simdjson.h"

using Config = simdjson::dom::element;

class ConfigManager {
public:
    ConfigManager(int argc, char* argv[]);
    ~ConfigManager();

    [[nodiscard]] Config config() const { return config_; }

    std::string service;
    std::string config_filename;
    const std::string program_name;

private:
    std::string           default_config_path_;
    simdjson::dom::parser parser_;
    Config                config_;

    void parse_arguments(int argc, char* argv[]);
    void create_config_file(std::string const& filepath);
    void parse_config_file();

    static void print_help(std::string const& program_name);
};

#endif //CONFIG_HH
