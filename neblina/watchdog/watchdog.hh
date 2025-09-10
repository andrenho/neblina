#ifndef WATCHDOG_HH
#define WATCHDOG_HH

#include <chrono>
#include <optional>

using namespace std::chrono_literals;
using sc = std::chrono::system_clock;

#include "service/service.hh"
#include "watchdog_config.hh"

class Watchdog : public Service {
public:
    explicit Watchdog(ConfigManager& config_manager);

    [[noreturn]] void run() override;

    static const char* name() { return "watchdog"; }

private:
    struct Service {
        WatchdogConfig::Service   details;
        std::optional<pid_t>      pid {};
        size_t                    attempts = 0;
        decltype(sc::now())       last_attempt = sc::now();
        std::chrono::milliseconds retry_in = 50ms;
    };

    ConfigManager& config_manager_;
    WatchdogConfig config_;
    std::vector<Service> services_;

    static bool service_is_running(Service& svc);
    bool service_eligible_for_retry(Service& svc);
    void start_service(Service& svc);

    static constexpr size_t MAX_ATTEMPTS = 10;
};

#endif //WATCHDOG_HH
