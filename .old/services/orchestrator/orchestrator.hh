#ifndef ORCHESTRATOR_HH
#define ORCHESTRATOR_HH

#include "arguments.hh"
#include "util/chrono.hh"
#include "service/scheduled/scheduled_service.hh"
#include "orchestrator_config.hh"

class Orchestrator : public ScheduledService {
public:
    SERVICE_NAME = "orchestrator";

    Orchestrator()
        : ScheduledService(100ms), config_(OrchestratorConfig::from_file(config_filename())) {}

    void init() override;

private:
    OrchestratorConfig config_;

    struct Service {
        OrchestratorConfig::Service config;
        std::optional<pid_t>        pid {};
        size_t                      attempts = 0;
        decltype(hr::now())         last_attempt = hr::now();
        std::chrono::milliseconds   retry_in = 50ms;
    };
    std::vector<Service> services_ {};

    void iteration() override;
    bool service_eligible_for_retry(Service const& svc);

    static fs::path config_filename() { return args().config_dir() / "orchestrator.json"; };
    static void     start_service(Service& svc);
    static bool     service_is_running(Service& svc);

    static constexpr size_t MAX_ATTEMPTS = 10;
    static constexpr auto   RESET_ATTEMPTS_IN = 5s;
};

#endif //ORCHESTRATOR_HH
