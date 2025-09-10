#include "watchdog.hh"

#include <chrono>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>

using namespace std::string_literals;
using namespace std::chrono_literals;

Watchdog::Watchdog(ConfigManager& config_manager)
    : config_manager_(config_manager), config_(config_manager)
{
    for (auto const& s: config_.services())
        services_.push_back({ .details = s });
}

[[noreturn]] void Watchdog::run()
{
    for (;;) {
        for (auto& svc: services_) {
            if (!service_is_running(svc) && service_eligible_for_retry(svc))
                start_service(svc);
        }
        std::this_thread::sleep_for(100ms);
    }
}

bool Watchdog::service_is_running(Service& svc)
{
    if (!svc.pid)
        return false;

    int status;
    pid_t done = waitpid(*svc.pid, &status, WNOHANG);
    if (done == *svc.pid) {
        svc.pid = {};
        svc.retry_in *= 2;
        std::cerr << "Service process '" << svc.details.name << "' has died with status " << WEXITSTATUS(status) << ". ";
        if (svc.attempts < MAX_ATTEMPTS)
            std::cerr << "Attempt " << svc.attempts << " in " << svc.retry_in << ".\n";
        else
            std::cerr << "Giving up.\n";
        return false;
    }

    return true;
}

bool Watchdog::service_eligible_for_retry(Service& svc)
{
    if (svc.attempts > MAX_ATTEMPTS) {
        services_.erase(
            std::remove_if(services_.begin(), services_.end(), [&](Service const& s) { return s.details.name == svc.details.name; }),
            services_.end());
        return false;
    }

    if (sc::now() < svc.last_attempt + svc.retry_in)
        return false;

    return true;
}

void Watchdog::start_service(Service& svc)
{
    pid_t pid = fork();

    ++svc.attempts;
    svc.last_attempt = sc::now();

    if (pid == 0) {   // child process
        std::cout << "Starting service " << svc.details.name << " with pid " << getpid() << "\n";
        execlp(config_.program_name().c_str(), config_.program_name().c_str(),
            "-C", config_.config_filename().c_str(),
            "-s", svc.details.name.c_str(), nullptr);
        throw std::runtime_error("execvp failed when starting a new service: "s + strerror(errno));
    } else if (pid > 0) {
        svc.pid.emplace(pid);
    } else {
        throw std::runtime_error("fork error");
    }
}
