#include "watchdog_config.hh"

std::vector<WatchdogConfig::Service> WatchdogConfig::services() const
{
    std::vector<Service> services;

    auto t_services = config_manager_.config()["watchdog"]["services"].get_array();
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

    return services;
}

std::string WatchdogConfig::config_filename() const
{
    return config_manager_.config_filename;
}

std::string WatchdogConfig::program_name() const
{
    return config_manager_.program_name;
}
