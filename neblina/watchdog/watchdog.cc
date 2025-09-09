#include "watchdog.hh"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

Watchdog::Watchdog(ConfigManager& cm)
    : config_(cm)
{
    for (auto const& s: config_.services)
        services_.push_back({ .details = s });
}

[[noreturn]] void Watchdog::run()
{
    for (;;) {
        for (auto const& svc: services_) {
            if (!service_is_running(svc) && service_eligible_for_retry(svc))
                start_service(svc);
        }
        std::this_thread::sleep_for(100ms);
    }
}

bool Watchdog::service_is_running(Watchdog::Service const& svc)
{
    if (!svc.pid)
        return false;
    // TODO - check actual service
    return true;
}

bool Watchdog::service_eligible_for_retry(Watchdog::Service const& svc)
{
    // TODO
    return true;
}

void Watchdog::start_service(Watchdog::Service const& svc)
{
    // TODO
}
