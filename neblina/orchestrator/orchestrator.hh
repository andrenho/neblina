#ifndef ORCHESTRATOR_HH
#define ORCHESTRATOR_HH

#include "service/scheduled_service.hh"

class Orchestrator : public ScheduledService {
public:
    Orchestrator()
        : ScheduledService(100ms) {}

protected:
    void iteration() override;

public:
    static constexpr std::string_view name = "orchestrator";
};

#endif //ORCHESTRATOR_HH
