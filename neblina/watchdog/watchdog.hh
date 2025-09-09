#ifndef WATCHDOG_HH
#define WATCHDOG_HH

#include <chrono>
#include <optional>

using namespace std::chrono_literals;
using sc = std::chrono::system_clock;

#include "watchdog_config.hh"

class Watchdog {
public:
    Watchdog(WatchdogConfig const& config, std::string const& program_name, std::string const& config_filename);

    [[noreturn]] void run();

private:
    struct Service {
        WatchdogConfig::Service   details;
        std::optional<pid_t>      pid {};
        size_t                    attempts = 0;
        decltype(sc::now())       last_attempt = sc::now();
        std::chrono::milliseconds retry_in = 50ms;
    };

    WatchdogConfig config_;
    std::vector<Service> services_;
    const std::string program_name_;
    const std::string config_filename_;

    static bool service_is_running(Service& svc);
    bool service_eligible_for_retry(Service& svc);
    void start_service(Service& svc);

    static constexpr size_t MAX_ATTEMPTS = 10;
};

#endif //WATCHDOG_HH
