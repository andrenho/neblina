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

    simdjson::dom::parser parser;
    simdjson::dom::element config;
    auto error = parser.load(config_filename_).get(config);
    if (error)
        throw std::runtime_error("Error parsing config file JSON");

    auto t_services = config["services"].get_array();
    if (t_services.error())
        throw std::runtime_error("Expected a 'services' key in config file");
    for (auto service: t_services) {
        Service svc = {
            .port = (uint16_t) service["port"].get_int64(),
            .open_to_world = service["open_to_world"].error() ? false : (bool) service["open_to_world"].get_bool()
        };
        if (!service["name"].get_string().get(svc.name) || svc.port == 0)
            throw std::runtime_error("Incorrect watchdog service configuration");
        services.emplace_back(std::move(svc));
    }

    return *this;
}