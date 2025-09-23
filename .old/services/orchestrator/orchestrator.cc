#include "orchestrator.hh"

#include <csignal>
#include <sys/wait.h>

#include <unistd.h>

#include "util/log.hh"
#include "util/string.hh"
#include "util/exceptions/non_recoverable_exception.hh"

void Orchestrator::init()
{
    for (auto const& s: config_.services)
        if (s.active)
            services_.push_back({ .config = s });
}

void Orchestrator::iteration()
{
    for (auto& svc: services_) {
        if (!service_is_running(svc) && service_eligible_for_retry(svc))
            start_service(svc);
        if (hr::now() - svc.last_attempt > RESET_ATTEMPTS_IN)
            svc.attempts = 0;
    }
    std::this_thread::sleep_for(100ms);
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
        ERR("Service process '{}' has died with status {}{}.", svc.config.name, WEXITSTATUS(status),
            WEXITSTATUS(status) == NON_RECOVERABLE_RETURN_CODE ? " (non-recoverable)" : "");
        if (WEXITSTATUS(status) == NON_RECOVERABLE_RETURN_CODE)
            svc.attempts = MAX_ATTEMPTS;
        if (svc.attempts < MAX_ATTEMPTS)
            ERR("Attempt {} in {} ({}).", svc.attempts, svc.retry_in, svc.config.name);
        else
            ERR("Giving up on '{}'.", svc.config.name);
        return false;
    }

    return true;
}

bool Orchestrator::service_eligible_for_retry(Service const& svc)
{
    if (svc.attempts >= MAX_ATTEMPTS) {
        std::erase_if(services_, [&](Service const& s) { return s.config.name == svc.config.name; });
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
        if (args().verbose)
            arguments.emplace_back("-v");
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
        LOG("Starting service {} with pid {}", svc.config.name, getpid());
        DBG("(command line: {})", pars);
        svc.pid.emplace(pid);
        setpgid(pid, getpid());
    } else {
        throw std::runtime_error("fork error");
    }
}
