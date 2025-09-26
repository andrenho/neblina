#include "orchestrator.h"

#include <time.h>

#include "common.h"
#include "os/os.h"
#include "util/error.h"

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

static void start_task_if_stopped(Task* task)
{
    if (task->pid == -1) {
        if (task->recent_attempts == MAX_ATTEMPS) {
            LOG("Giving up on service '%s'", task->service->name);
            ++task->recent_attempts;
        } else if (task->recent_attempts < MAX_ATTEMPS) {
            LOG("Starting service '%s' with (attempt %d)", task->service->name, task->recent_attempts);
            task->pid = os_start_service(task->service);
            if (task->pid == 0) {
                ERR("Could not start process '%s': %s", task->service->name, last_error);
                return;
            }
            LOG("Service '%s' started with pid %d", task->service->name, task->pid);
            time(&task->last_attempt);
            ++task->recent_attempts;
        }
    }
}

static void mark_task_as_terminated_if_dead(Task *task)
{
    int status;
    if (!os_process_still_running(task->pid, &status)) {
        LOG("Sevice process '%s' has died with status %d%s", task->service->name, status,
            status == NON_RECOVERABLE_ERROR ? " (non-recoverable)" : "");
        task->pid = NOT_RUNNING;
        if (status == NON_RECOVERABLE_ERROR)
            task->recent_attempts = 10;
    }
}

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
        for (size_t i = 0; i < n_tasks; ++i)
            start_task_if_stopped(&tasks[i]);

        // check if any services have died
        for (size_t i = 0; i < n_tasks; ++i)
            mark_task_as_terminated_if_dead(&tasks[i]);

        // reset recent attempts
        time_t now; time(&now);
        for (size_t i = 0; i < n_tasks; ++i)
            if (tasks[i].recent_attempts >= MAX_ATTEMPS && difftime(now, tasks[i].last_attempt) > 10)
                tasks[i].recent_attempts = 0;

        os_sleep_ms(100);
    }
}
