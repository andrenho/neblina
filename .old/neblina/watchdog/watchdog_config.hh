#ifndef WATCHDOG_CONFIG_HH
#define WATCHDOG_CONFIG_HH

#include <vector>
#include <string>

#include "service/config.hh"

class WatchdogConfig : public Config {
public:
    explicit WatchdogConfig(std::string const& config_file_path);

    std::vector<std::string> services;

    WatchdogConfig& load() override;
};

#endif //WATCHDOG_CONFIG_HH
