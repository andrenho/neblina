#ifndef NEBLINA_TCP_SERVER_H
#define NEBLINA_TCP_SERVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "connection.h"

#define DFLT_CONN_BUF_SZ 64
#define BUFFER_SZ (8 * 1024)

void tcp_server_start(int port, bool open_to_world, DataType data_type);

#endif //NEBLINA_TCP_SERVER_H
