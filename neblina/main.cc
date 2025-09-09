#include <iostream>

#include "util/config/config_manager.hh"
#include "watchdog/watchdog.hh"

int main(int argc, char* argv[])
{
    ConfigManager config_manager(argc, argv);
    auto config = config_manager.parse_config_file();

    if (config_manager.service.empty())
        Watchdog(WatchdogConfig(config), config_manager.program_name, config_manager.config_filename).run();
    else
        std::cout << "Started service " << config_manager.service << std::endl;
}
