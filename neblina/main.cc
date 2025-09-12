#define SERVICES Orchestrator

#include "arguments.hh"
#include "orchestrator/orchestrator.hh"
#include "service/registry.hh"
#include "util/exceptions/non_recoverable_exception.hh"

#include <iostream>

int main(int argc, char* argv[])
{
    args(argc, argv);

    // register services
    ServiceRegistry services;
    register_native_services<SERVICES>(services);

    // find and execute service
    std::cout << "Starting service " << args().service << "..." << std::endl;  // TODO - replace with logging
    auto it = services.find(args().service);

    try {
        if (it != services.end()) {
            auto service = it->second();
            service->init();
            service->run();
        }
        throw NonRecoverableException(args().service + " not a native service, and contributed services are not yet implemented.");

    } catch (NonRecoverableException& e) {
        std::cerr << e.what() << std::endl;
        return NON_RECOVERABLE_RETURN_CODE;
    }
}