#ifndef SCHEDULED_SERVICE_HH
#define SCHEDULED_SERVICE_HH

#include "../service.hh"
#include "util/chrono.hh"

class ScheduledService : public Service {
public:
    explicit ScheduledService(std::chrono::milliseconds frequency) : frequency_(frequency) {}

    [[noreturn]] void run() override;

protected:
    std::chrono::milliseconds frequency_;

    virtual void iteration() = 0;
};


#endif //SCHEDULED_SERVICE_HH
