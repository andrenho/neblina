#ifndef NEBLINA_CONNECTION_H
#define NEBLINA_CONNECTION_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "socket.h"
#include "uthash.h"
#include "session.h"

typedef struct {
    SOCKET         fd;          // key
    void*          session;     // Session provided by the service
    UT_hash_handle hh;          // UTHash handle to use this structure as a map
} Connection;


#endif //NEBLINA_CONNECTION_H
