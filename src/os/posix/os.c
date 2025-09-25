#include "os/os.h"

#include "common.h"

#include <signal.h>
#include <unistd.h>

void handle_sigint(int signum)
{
    (void) signum;
    termination_requested = true;
}

void os_handle_ctrl_c()
{
    // signal to catch CTRL+C and exit gracefully
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
}

void os_sleep_ms(size_t ms)
{
    usleep(ms * 1000);
}

pid_t os_start_service(ConfigService const* service)
{
    return 0;   // TODO
}

bool os_process_still_running(pid_t pid)
{
    return false;   // TODO
}