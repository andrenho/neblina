#include "watchdog_config.hh"

WatchdogConfig::WatchdogConfig(Config const& config)
{
    auto t_services = config["watchdog"]["services"].get_array();
    if (t_services.error())
        throw std::runtime_error("Expected a 'services' key in config file");
    for (auto service: t_services) {
        Service svc = {
            .name = std::string(service["name"]),
            .port = (uint16_t) service["port"].get_int64(),
            .open_to_world = service["open_to_world"].get_bool(),
        };
        if (svc.name.empty() || svc.port == 0)
            throw std::runtime_error("Incorrect watchdog service configuration");
        services.emplace_back(std::move(svc));
    };
}
