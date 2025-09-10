#include <iostream>

#include "parrot/parrot_char.hh"
#include "util/config/config_manager.hh"
#include "watchdog/watchdog.hh"
#include "register.hh"

int main(int argc, char* argv[])
{
    // register services
    ServiceRegistry services;
    register_native_services<Watchdog>(services);

    // initialize utility classes
    ConfigManager config_manager(argc, argv);

    // find and execute service
    std::cout << "Starting service " << config_manager.service << "..." << std::endl;
    auto it = services.find(config_manager.service);
    if (it != services.end())  // native service
        it->second(config_manager)->run();

    throw std::runtime_error(config_manager.service + " not a native service, and contributed services are not yet implemented.");
}
