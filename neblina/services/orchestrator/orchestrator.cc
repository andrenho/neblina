#include "orchestrator.hh"

#include <csignal>
#include <sys/wait.h>

#include <string>
#include <unistd.h>
using namespace std::string_literals;

#include "util/log.hh"
#include "util/exceptions/non_recoverable_exception.hh"

void Orchestrator::init()
{
    for (auto const& s: config_.services)
        if (s.active)
            services_.push_back({ .config = s });
}

OrchestratorConfig Orchestrator::load_config_file()
{
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
        // TODO - reset attemps if last attempt was a while ago
        err("Service process {} has died with status {}{}.", svc.config.name, WEXITSTATUS(status),
            WEXITSTATUS(status) == NON_RECOVERABLE_RETURN_CODE ? " (non-recoverable)" : "");
        if (WEXITSTATUS(status) == NON_RECOVERABLE_RETURN_CODE)
            svc.attempts = MAX_ATTEMPTS;
        if (svc.attempts < MAX_ATTEMPTS)
            err("Attempt {} in {}.", svc.attempts, svc.retry_in);
        else
            err("Giving up.");
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

    std::vector arguments = {
        args().program_name.c_str(),
        "-D", args().data_dir.c_str(),
        "-s", svc.config.name.c_str(),
    };

    std::string s_port;
    if (svc.config.port) {
        arguments.emplace_back("-P");
        s_port = std::to_string(*svc.config.port);
        arguments.emplace_back(s_port.c_str());
        if (svc.config.open_to_world && *svc.config.open_to_world)
            arguments.emplace_back("-w");
    }
    if (svc.config.logging_color) {
        arguments.emplace_back("-c");
        arguments.emplace_back(svc.config.logging_color->c_str());
    }

    std::string pars; for (auto const& i: arguments) pars += std::string(i) + ' ';
    arguments.emplace_back(nullptr);

    if (pid == 0) {   // child process
        execvp(args().program_name.c_str(), (char* const*) arguments.data());
        throw std::runtime_error("execvp failed when starting a new service: "s + strerror(errno));

    } else if (pid > 0) {
        log("Starting service {} with pid {} ({}).", svc.config.name, getpid(), pars);
        svc.pid.emplace(pid);
        setpgid(pid, getpid());
    } else {
        throw std::runtime_error("fork error");
    }
}
