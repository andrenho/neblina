#define SERVICES Watchdog

#include <iostream>

#include "watchdog/watchdog.hh"
#include "register.hh"
#include "util/exceptions/non_recoverable_exception.hh"
#include "arguments.hh"

int main(int argc, char* argv[])
{
    Arguments args(argc, argv);

    // register services
    ServiceRegistry services;
    register_native_services<SERVICES>(services);

    // find and execute service
    std::cout << "Starting service " << args.service << "..." << std::endl;
    auto it = services.find(args.service);

    try {
        if (it != services.end())  // native service
            it->second(args)->run();
        throw NonRecoverableException(args.service + " not a native service, and contributed services are not yet implemented.");

    } catch (NonRecoverableException& e) {
        std::cerr << e.what() << std::endl;
        return NON_RECOVERABLE_RETURN_CODE;
    }
}
