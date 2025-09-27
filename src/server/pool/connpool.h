#ifndef NEBLINA_CONNPOOL_H
#define NEBLINA_CONNPOOL_H

#include <stddef.h>
#include "../connection.h"

void connpool_init(size_t n_threads, Connection** connection_set);
void connpool_ready(SOCKET fd);

#endif //NEBLINA_CONNPOOL_H
