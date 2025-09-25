#include "orchestrator.h"

#include <time.h>

#include "common.h"
#include "os/os.h"
#include "util/ntime.h"

#define NOT_RUNNING (-1)

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
        tasks[i] = (Task) {
            .service = &main_config.services[i],
            .pid = NOT_RUNNING,
            .recent_attempts = 0,
        };
        time(&tasks[i].last_attempt);
    }
    n_tasks = main_config.services_sz;

    // keep track of services, restart if down
    for (;;) {
        // start/restart stopped services
        for (size_t i = 0; i < n_tasks; ++i) {
            if (tasks[i].pid == -1) {
                tasks[i].pid = os_start_service(tasks[i].service);
                time(&tasks[i].last_attempt);
                ++tasks[i].recent_attempts;
            }
        }

        // check if any services have died
        for (size_t i = 0; i < n_tasks; ++i)
            if (!os_process_still_running(tasks[i].pid))
                tasks[i].pid = NOT_RUNNING;

        // reset recent attempts
        time_t now; time(&now);
        for (size_t i = 0; i < n_tasks; ++i)
            if (time_diff_ms(tasks[i].last_attempt, now) > 10000)
                tasks[i].recent_attempts = 0;

        os_sleep_ms(100);
    }
}
