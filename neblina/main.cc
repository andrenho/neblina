#include <iostream>
#include <functional>

#include "parrot/parrot_char.hh"
#include "util/config/config_manager.hh"
#include "watchdog/watchdog.hh"

static std::unordered_map<std::string, std::function<std::unique_ptr<Service>(int port, bool open_to_world)>> services;

template <typename T>
void register_native_service()
{
    services[T::name()] = [](int port, bool open_to_world){ return std::make_unique<T>(port, open_to_world); };
}

int main(int argc, char* argv[])
{
    ConfigManager config_manager(argc, argv);
    Config const& config = config_manager.config();

    if (config_manager.service.empty()) {
        Watchdog(WatchdogConfig(config), config_manager.program_name, config_manager.config_filename).run();
    } else {
        register_native_service<ParrotChar>();
        std::cout << "Started service " << config_manager.service << std::endl;
    }
}
