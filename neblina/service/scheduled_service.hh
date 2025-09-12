#ifndef SCHEDULED_SERVICE_HH
#define SCHEDULED_SERVICE_HH

#include "service.hh"
#include "util/chrono.hh"

class ScheduledService : public Service {
public:
    explicit ScheduledService(duration_ms frequency) : frequency_(frequency) {}

    void run() override;

protected:
    duration_ms frequency_;

    virtual void iteration() = 0;
};


#endif //SCHEDULED_SERVICE_HH
