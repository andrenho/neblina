#include "connpool.h"

#include <pthread.h>

typedef struct {
    SOCKET fd;
    void*  session;
} Task;

void connpool_init(size_t n_threads)
{
    (void) n_threads;
}

void connpool_add_task(SOCKET fd, uint8_t* data, size_t data_sz, ProcessSessionF process_cb, void* session, SendF send_f)
{
    process_cb(session, data, data_sz, send_f, fd);
    free(data);
}

void connpoll_finalize()
{
}
