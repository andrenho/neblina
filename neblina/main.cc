#define SERVICES Orchestrator, Parrot, Http

#include <filesystem>
namespace fs = std::filesystem;

#include "arguments.hh"
#include "file/fileset.hh"
#include "util/exceptions/non_recoverable_exception.hh"

// native services
#include "services/orchestrator/orchestrator.hh"
#include "services/parrot/parrot.hh"
#include "services/http/http.hh"

#include "init.gen.inc"

#define SERVICE_NOT_FOUND 300

template <typename T>
int find_and_execute(std::string const& service_name)
{
    if (T::name == service_name) {
        auto service = T();
        try {
            // Run the service. In case of failure the orchestrator will restart the service.
            service.init();
            service.run();
            return EXIT_FAILURE;

        } catch (NonRecoverableException& e) {
            // This exception means it's no use to retry (ex. a wrong configuration file).
            LOG("{}", e.what()); fflush(stderr);
            return NON_RECOVERABLE_RETURN_CODE;
        }
    }

    return SERVICE_NOT_FOUND;
}

template <typename... Ts>
int find_and_execute_service(std::string const& service_name)
{
    return (find_and_execute<Ts>(service_name), ...);
}

int main(int argc, char* argv[])
{
    // read and parse command-line arguments
    args(argc, argv);

    // create initial directory setup
    if (!fs::exists(args().data_dir)) {
        try {
            deploy_fileset(init, args().data_dir);
            LOG("New fileset deployed to {}", args().data_dir.string());
        } catch (std::exception& e) {
            ERR("There was an error trying to deploy the initial file configuration: {}", e.what());
            return EXIT_FAILURE;
        }
    }

    // execute service
    int r = find_and_execute_service<SERVICES>(args().service);
    if (r == SERVICE_NOT_FOUND) {
        ERR("Service {} was not found.", args().service);
        return NON_RECOVERABLE_RETURN_CODE;
    }
    return r;
}
