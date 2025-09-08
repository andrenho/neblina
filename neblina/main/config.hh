#ifndef CONFIG_HH
#define CONFIG_HH

#include <string>

class Config {
public:
    Config(int argc, char* argv[]);

private:
    std::string config_filename;

    void parse_arguments(int argc, char* argv[]);
    void create_config_file();
    void load_config_file();
};

#endif //CONFIG_HH
