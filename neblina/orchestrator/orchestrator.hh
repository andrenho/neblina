#ifndef ORCHESTRATOR_HH
#define ORCHESTRATOR_HH

#include "arguments.hh"
#include "service/scheduled_service.hh"
#include "orchestrator_config.hh"

class Orchestrator : public ScheduledService {
public:
    Orchestrator()
        : ScheduledService(100ms), config_(OrchestratorConfig::from_file(args().data_dir + "/orchestrator.json")) {}

    void init() override;

    static constexpr std::string_view name = "orchestrator";

private:
    OrchestratorConfig config_;

    void iteration() override;
};

#endif //ORCHESTRATOR_HH
