#ifndef NEBLINA_TCP_SERVER_H
#define NEBLINA_TCP_SERVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t* input_queue;
    size_t   input_queue_sz;
    void*    data;
} Connection;

typedef void(*CreateConnectionF)(Connection* c);
typedef void(*ProcessConnectionF)(Connection* c);

void tcp_server_start(int port, bool open_to_world, CreateConnectionF cf, ProcessConnectionF pf);

#endif //NEBLINA_TCP_SERVER_H
