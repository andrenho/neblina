#ifndef NEBLINA_TCP_SERVER_H
#define NEBLINA_TCP_SERVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define DFLT_CONN_BUF_SZ 64
#define BUFFER_SZ 8 * 1024

typedef struct {
    uint8_t* input_queue;
    size_t   input_queue_sz;
    size_t   reserved_sz;
    bool     new_data;
} Connection;

typedef enum { D_BINARY, D_TEXT } DataType;

void tcp_server_start(int port, bool open_to_world, DataType data_type);

#endif //NEBLINA_TCP_SERVER_H
