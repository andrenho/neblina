#ifndef CONFIG_HH
#define CONFIG_HH

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace toml::inline v3 { class table; }

class ConfigManager {
public:
    ConfigManager(int argc, char* argv[]);
    ~ConfigManager();

    toml::v3::table& toml() { return *toml_; }
    [[nodiscard]] toml::v3::table const& toml() const { return *toml_; }

private:
    void parse_arguments(int argc, char* argv[]);
    void create_config_file(std::string const& filepath);
    static void print_help(std::string const& program_name);

    std::string default_config_path_;
    std::string config_filename_;
    std::unique_ptr<toml::v3::table> toml_;
};

#endif //CONFIG_HH
