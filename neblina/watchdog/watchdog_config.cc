#include "watchdog_config.hh"

#include <contrib/toml/toml.hpp>

WatchdogConfig::WatchdogConfig(ConfigManager const& cm)
{
    auto t_service_array = cm.toml()["watchdog"]["services"].as_array();
    if (!t_service_array)
        throw std::runtime_error("Expected a 'services' key in config file");
    for (auto&& e: *t_service_array) {
        auto tbl = e.as_table();
        Service svc = {
            .name = tbl->at("name").value_or(""),
            .port = (uint16_t) tbl->at("port").value_or(0),
            .open_to_world = tbl->at("port").value_or(false),
        };
        if (svc.name.empty() || svc.port == 0)
            throw std::runtime_error("Incorrect watchdog service configuration");
        services.emplace_back(std::move(svc));
    };
}
