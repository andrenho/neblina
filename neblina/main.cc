#include <iostream>

#include "util/config/config_manager.hh"
#include "util/tcp/tcp_server.hh"
#include "watchdog/watchdog.hh"

int main(int argc, char* argv[])
{
    /*
    ConfigManager config(argc, argv);
    if (config.service.empty())
        Watchdog(config).run();
    else
        std::cout << "Started service " << config.service << std::endl;
    */
    TCPServer t(23456);
    t.run();
}
