#include <iostream>

#include "util/config_manager.hh"
#include "watchdog/watchdog.hh"

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << " ";
    std::cout << "\n";

    ConfigManager config(argc, argv);
    if (config.service.empty())
        Watchdog(config).run();
    else
        std::cout << "Started service " << config.service << std::endl;
}
