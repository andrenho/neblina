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
        : ScheduledService(100ms), config_(load_config_file()) {}

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

    static bool service_is_running(Service& svc);
    bool service_eligible_for_retry(Service& svc);
    void start_service(Service& svc);

    static OrchestratorConfig load_config_file();
    static std::string        config_filename() { return args().data_dir + "/orchestrator.json"; };

    static constexpr size_t MAX_ATTEMPTS = 10;
};

#endif //ORCHESTRATOR_HH
