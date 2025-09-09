#include "util/config_manager.hh"
#include "watchdog/watchdog.hh"

int main(int argc, char* argv[])
{
    ConfigManager config(argc, argv);
    Watchdog(config).run();
}
