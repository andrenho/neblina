#ifndef WATCHDOG_CONFIG_HH
#define WATCHDOG_CONFIG_HH

#include <cstdint>
#include <vector>
#include <string>

#include "util/config/config_manager.hh"

class WatchdogConfig {
public:
    explicit WatchdogConfig(ConfigManager& config_manager) : config_manager_(config_manager) {}

    struct Service {
        std::string name;
        uint16_t    port;
        bool        open_to_world;
    };
    [[nodiscard]] std::vector<Service> services() const;

    [[nodiscard]] std::string config_filename() const;
    [[nodiscard]] std::string program_name() const;

private:
    ConfigManager const& config_manager_;
};

#endif //WATCHDOG_CONFIG_HH
