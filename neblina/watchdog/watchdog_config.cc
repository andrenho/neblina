#include "watchdog_config.hh"

#include "contrib/simdjson/simdjson.h"
#include "watchdog/watchdog.gen.inc"

WatchdogConfig::WatchdogConfig(std::string const& config_file_path)
    : Config(config_file_path, "watchdog.json", watchdog, sizeof watchdog, watchdog_uncompressed_sz)
{
}

WatchdogConfig& WatchdogConfig::load()
{
    // TODO - use ondemand (?)

    simdjson::ondemand::parser parser;
    auto json = simdjson::padded_string::load(config_filename_);
    if (json.error())
        throw std::runtime_error("Error parsing config file JSON");
    auto doc = parser.iterate(json);

    auto t_services = doc["services"].get_array();
    if (t_services.error())
        throw std::runtime_error("Expected a 'services' key in config file");
    for (auto service: t_services) {
        std::string service_name;
        service.get_string(service_name);
        services.push_back(service_name);
    }

    return *this;
}