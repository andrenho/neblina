#include <iostream>

#include "util/config_manager.hh"
#include "watchdog/watchdog.hh"

int main(int argc, char* argv[])
{
    ConfigManager config(argc, argv);
    if (config.service.empty())
        Watchdog(config).run();
    else
        std::cout << "Started service " << config.service << std::endl;
}
