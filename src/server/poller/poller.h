#ifndef NEBLINA_POLLER_H
#define NEBLINA_POLLER_H

#include <stdbool.h>
#include <stddef.h>

#ifndef _WIN32
typedef int SOCKET;
#endif

typedef enum { PT_NEW_CONNECTION, PT_NEW_DATA, PT_DISCONNECTED } PollerEventType;
typedef struct {
    PollerEventType type;
    int             fd;
} PollerEvent;

#define TIMEOUT 100

void   poller_init(SOCKET fd_listener);
size_t poller_wait(PollerEvent* out_evt, size_t evt_sz);
bool   poller_add_connection(SOCKET fd);

#endif //NEBLINA_POLLER_H
