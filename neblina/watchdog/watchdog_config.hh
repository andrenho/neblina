#ifndef WATCHDOG_CONFIG_HH
#define WATCHDOG_CONFIG_HH

#include <cstdint>
#include <vector>
#include <string>

#include "service/config.hh"

class WatchdogConfig : public Config {
public:
    explicit WatchdogConfig(std::string const& config_file_path);

    struct Service {
        std::string name;
        uint16_t    port;
        bool        open_to_world;
    };
    std::vector<Service> services;

    WatchdogConfig& load() override;
};

#endif //WATCHDOG_CONFIG_HH
