#include "watchdog_config.hh"

WatchdogConfig::WatchdogConfig(Config& config_)
{
    simdjson::dom::parser parser;
    simdjson::dom::element config;
    auto error = parser.load("/home/andre/.config/neblina/neblina.json").get(config);
    if (error)
        throw std::runtime_error("Error parsing config file JSON");

    auto t_services = config["watchdog"]["services"].get_array();
    if (t_services.error())
        throw std::runtime_error("Expected a 'services' key in config file");
    for (auto service: t_services) {
        Service svc = {
            .name = std::string(service["name"]),
            .port = (uint16_t) int(service["port"]),
            .open_to_world = bool(service["open_to_world"])
        };
        if (svc.name.empty() || svc.port == 0)
            throw std::runtime_error("Incorrect watchdog service configuration");
        services.emplace_back(std::move(svc));
    };
}
