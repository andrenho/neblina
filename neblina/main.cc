#define SERVICES Orchestrator, Parrot

#include "arguments.hh"
#include "services/orchestrator/orchestrator.hh"
#include "services/parrot/parrot.hh"
#include "util/exceptions/non_recoverable_exception.hh"

#include "util/log.hh"

template <typename T>
int find_and_execute(std::string const& service_name)
{
    if (T::name == service_name) {
        auto service = T();
        try {
            service.init();
            service.run();
            throw NonRecoverableException("not a native service, and contributed services are not yet implemented.");

        } catch (NonRecoverableException& e) {
            log("{}", e.what()); fflush(stderr);
            return NON_RECOVERABLE_RETURN_CODE;
        }
    }

    return 0;
}

template <typename... Ts>
int find_and_execute_service(std::string const& service_name)
{
    return (find_and_execute<Ts>(service_name), ...);
}

int main(int argc, char* argv[])
{
    args(argc, argv);
    return find_and_execute_service<SERVICES>(args().service);
}
