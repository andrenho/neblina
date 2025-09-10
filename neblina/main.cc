#include <iostream>

#include "util/config/config_manager.hh"
#include "watchdog/watchdog.hh"

int main(int argc, char* argv[])
{
    ConfigManager config_manager(argc, argv);
    Config const& config = config_manager.config();

    if (config_manager.service.empty())
        Watchdog(WatchdogConfig(config), config_manager.program_name, config_manager.config_filename).run();
    else
        std::cout << "Started service " << config_manager.service << std::endl;
}
