#include "watchdog.hh"

#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

Watchdog::Watchdog(ConfigManager& cm)
    : config_(cm), program_name_(cm.program_name), config_filename_(cm.config_filename)
{
    for (auto const& s: config_.services)
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

bool Watchdog::service_is_running(Service const& svc)
{
    if (!svc.pid)
        return false;
    // TODO - check actual service
    return true;
}

bool Watchdog::service_eligible_for_retry(Service const& svc)
{
    // TODO
    return true;
}

void Watchdog::start_service(Service& svc)
{
    pid_t pid = fork();

    if (pid == 0) {   // child process
        std::cout << "Starting service " << svc.details.name << " with pid " << getpid() << "\n";

        char* args[] = {
            (char *) program_name_.c_str(),
            (char *) "-C",
            (char *) config_filename_.c_str(),
            (char *) "-s",
            (char *) svc.details.name.c_str(),
        };
        execvp(args[0], args);

        throw std::runtime_error("execvp failed when starting a new service");
    } else if (pid > 0) {
        svc.pid.emplace(pid);
    } else {
        throw std::runtime_error("fork error");
    }
}
