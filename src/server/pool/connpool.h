#ifndef NEBLINA_CONNPOOL_H
#define NEBLINA_CONNPOOL_H

#include <stddef.h>
#include "../connection.h"

void connpool_init(size_t n_threads, Connection** connection_set);
void connpool_add_task(SOCKET fd, uint8_t* data, size_t data_sz, ProcessSessionF process_cb, void* session, SendF send_f);
void connpoll_finalize();

#endif //NEBLINA_CONNPOOL_H
