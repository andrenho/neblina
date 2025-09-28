#include "parrot.h"

#include "common.h"
#include "server/tcp_server.h"

static void parrot_process(void* session, uint8_t const* data, size_t data_sz, SendF send_f, SOCKET fd)
{
    (void) session;
    send_f(fd, data, data_sz);
    send_f(fd, (uint8_t const *) "\r\n", 2);
}

void start_parrot()
{
    SessionCallbacks session_cb = {
            .create_session = NULL,
            .process_session = parrot_process,
            .destroy_session = NULL,
    };
    tcp_server_start(args.port, args.open_to_world, session_cb);
}