#ifndef WATCHDOG_CONFIG_HH
#define WATCHDOG_CONFIG_HH

#include <cstdint>
#include <vector>
#include <string>

#include "util/config_manager.hh"

struct WatchdogConfig {
    explicit WatchdogConfig(ConfigManager const& cm);

    struct Service {
        std::string name;
        uint16_t    port;
        bool        open_to_world;
    };
    std::vector<Service> services;
};

#endif //WATCHDOG_CONFIG_HH
