#ifndef NEBLINA_OS_H
#define NEBLINA_OS_H

#include <stddef.h>

#include "common.h"

void os_handle_ctrl_c();
void os_sleep_ms(size_t ms);

pid_t os_start_service(ConfigService const* service);
bool  os_process_still_running(pid_t pid);

#endif //NEBLINA_OS_H
