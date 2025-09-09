#ifndef WATCHDOG_HH
#define WATCHDOG_HH

#include <optional>

#include "watchdog_config.hh"

class Watchdog {
public:
    explicit Watchdog(ConfigManager& cm);

    [[noreturn]] void run();

private:
    struct Service {
        WatchdogConfig::Service details;
        std::optional<pid_t>    pid {};
        size_t                  attempts = 0;
    };

    WatchdogConfig config_;
    std::vector<Service> services_;
    const std::string program_name_;
    const std::string config_filename_;

    bool service_is_running(Service const& svc);
    bool service_eligible_for_retry(Service const& svc);
    void start_service(Service& svc);
};

#endif //WATCHDOG_HH
