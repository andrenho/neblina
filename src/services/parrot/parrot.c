#include "parrot.h"

#include "common.h"
#include "server/tcp_server.h"

void start_parrot()
{
    tcp_server_start(args.port, args.open_to_world, NULL, NULL);
}