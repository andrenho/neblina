#ifndef CONFIG_HH
#define CONFIG_HH

#include <cstdint>
#include <string>
#include <unordered_map>

class Config {
public:
    Config(int argc, char* argv[]);

    std::unordered_map<std::string, uint16_t> services;

private:
    std::string config_filename;

    void parse_arguments(int argc, char* argv[]);
    void create_config_file(std::string const& filepath);
    void load_config_file();
    static std::string default_config_file_path();
};

#endif //CONFIG_HH
