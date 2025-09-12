#include "scheduled_service.hh"

#include <thread>

void ScheduledService::run()
{
    auto last_execution = hr::now() - frequency_;

    for (;;) {
        auto current_time = hr::now();
        auto next_execution = last_execution + frequency_;
        if (current_time >= next_execution) {
            iteration();
            last_execution = current_time;
        } else {
            std::this_thread::sleep_for(next_execution - hr::now());
        }
    }
}
