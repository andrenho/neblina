#include "orchestrator.hh"

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <util/exceptions/non_recoverable_exception.hh>
using namespace std::string_literals;

#include "orchestrator/orchestrator.gen.inc"

void Orchestrator::init()
{
    for (auto const& s: config_.services)
        services_.push_back({ .config = s });
}

OrchestratorConfig Orchestrator::load_config_file()
{
    create_file_if_it_doesnt_exist(config_filename(), { orchestrator, orchestrator_uncompressed_sz, sizeof orchestrator });
    return OrchestratorConfig::from_file(config_filename());
}

void Orchestrator::iteration()
{
    for (;;) {
        for (auto& svc: services_) {
            if (!service_is_running(svc) && service_eligible_for_retry(svc))
                start_service(svc);
        }
        std::this_thread::sleep_for(100ms);
    }
}

bool Orchestrator::service_is_running(Service& svc)
{
    if (!svc.pid)
        return false;

    int status;
    pid_t done = waitpid(*svc.pid, &status, WNOHANG);
    if (done == *svc.pid) {
        svc.pid = {};
        svc.retry_in *= 2;
        if (WEXITSTATUS(status) == 0) {
            std::cerr << "Service process " << svc.config.name << " has finalized successfully.\n";
            svc.attempts = MAX_ATTEMPTS;
        } else {
            std::cerr << "Service process '" << svc.config.name << "' has died with status " << WEXITSTATUS(status) << ". ";
            if (WEXITSTATUS(status) == NON_RECOVERABLE_RETURN_CODE) {
                std::cerr << " (non-recoverable) ";
                svc.attempts = MAX_ATTEMPTS;
            }
            if (svc.attempts < MAX_ATTEMPTS)
                std::cerr << "Attempt " << svc.attempts << " in " << svc.retry_in << ".\n";
            else
                std::cerr << "Giving up.\n";
        }
        return false;
    }

    return true;
}

bool Orchestrator::service_eligible_for_retry(Service& svc)
{
    if (svc.attempts >= MAX_ATTEMPTS) {
        services_.erase(
            std::remove_if(services_.begin(), services_.end(), [&](Service const& s) { return s.config.name == svc.config.name; }),
            services_.end());
        return false;
    }

    if (hr::now() < svc.last_attempt + svc.retry_in)
        return false;

    return true;
}

void Orchestrator::start_service(Service& svc)
{
    pid_t pid = fork();

    ++svc.attempts;
    svc.last_attempt = hr::now();

    if (pid == 0) {   // child process
        std::cout << "Starting service " << svc.config.name << " with pid " << getpid() << "\n";

        std::vector arguments = {
            args().program_name.c_str(),
            "-D", args().data_dir.c_str(),
            "-s", svc.config.name.c_str(),
        };

        if (svc.config.port) {
            arguments.emplace_back("-P");
            arguments.emplace_back(std::to_string(*svc.config.port).c_str());
            if (svc.config.open_to_world && *svc.config.open_to_world)
                arguments.emplace_back("-w");
        }

        arguments.emplace_back(nullptr);
        execvp(args().program_name.c_str(), (char* const*) arguments.data());
        throw std::runtime_error("execvp failed when starting a new service: "s + strerror(errno));

    } else if (pid > 0) {
        svc.pid.emplace(pid);
    } else {
        throw std::runtime_error("fork error");
    }
}
