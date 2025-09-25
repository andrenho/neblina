#include "orchestrator.h"

#include <time.h>

#include "common.h"
#include "os/os.h"

#define NOT_RUNNING (-1)
#define MAX_ATTEMPS 10

typedef struct {
    ConfigService const* service;
    pid_t                pid;
    int                  recent_attempts;
    time_t               last_attempt;
} Task;

static Task   tasks[MAX_SERVICES];
static size_t n_tasks = 0;

void orchestrator_start()
{
    // create list of services
    for (int i = 0; i < main_config.services_sz; ++i) {
        if (main_config.services[i].active) {
            tasks[n_tasks++] = (Task) {
                .service = &main_config.services[i],
                .pid = NOT_RUNNING,
                .recent_attempts = 0,
            };
            time(&tasks[i].last_attempt);
        }
    }

    // keep track of services, restart if down
    while (!termination_requested) {
        // start/restart stopped services
        for (size_t i = 0; i < n_tasks; ++i) {
            if (tasks[i].pid == -1) {
                if (tasks[i].recent_attempts == MAX_ATTEMPS) {
                    LOG("Giving up on service '%s'", tasks[i].service->name);
                    ++tasks[i].recent_attempts;
                } else if (tasks[i].recent_attempts < MAX_ATTEMPS) {
                    LOG("Starting service '%s' with (attempt %d)", tasks[i].service->name, tasks[i].recent_attempts);
                    tasks[i].pid = os_start_service(tasks[i].service);
                    LOG("Service '%s' started with pid %d", tasks[i].service->name, tasks[i].pid);
                    time(&tasks[i].last_attempt);
                    ++tasks[i].recent_attempts;
                }
            }
        }

        // check if any services have died
        for (size_t i = 0; i < n_tasks; ++i)
            if (!os_process_still_running(tasks[i].pid))
                tasks[i].pid = NOT_RUNNING;

        // reset recent attempts
        time_t now; time(&now);
        for (size_t i = 0; i < n_tasks; ++i)
            if (tasks[i].recent_attempts >= MAX_ATTEMPS && difftime(now, tasks[i].last_attempt) > 10)
                tasks[i].recent_attempts = 0;

        os_sleep_ms(100);
    }
}
