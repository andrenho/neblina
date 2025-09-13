#define SERVICES Orchestrator

#include "arguments.hh"
#include "service/registry.hh"
#include "services/orchestrator/orchestrator.hh"
#include "util/exceptions/non_recoverable_exception.hh"

#include <iostream>

#include "util/log.hh"

int main(int argc, char* argv[])
{
    args(argc, argv);

    // register services
    ServiceRegistry services;
    register_native_services<SERVICES>(services);

    // find and execute service
    auto const it = services.find(args().service);

    try {
        if (it != services.end()) {
            auto const service = it->second();
            service->init();
            service->run();
        }
        throw NonRecoverableException("not a native service, and contributed services are not yet implemented.");

    } catch (NonRecoverableException& e) {
        log("{}", e.what()); fflush(stderr);
        return NON_RECOVERABLE_RETURN_CODE;
    }
}
