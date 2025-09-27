#include "parrot.h"

#include "common.h"
#include "server/tcp_server.h"

static void parrot_process(Session* session, uint8_t const* data, size_t data_sz, SendF send_f, void* ctx)
{
    (void) session;
    send_f(data, data_sz, ctx);
    send_f((uint8_t const *) "\r\n", 2, ctx);
}

void start_parrot()
{
    SessionCallbacks session_def = {
            .create_session = NULL,
            .process_session = parrot_process,
            .destroy_session = NULL,
    };
    tcp_server_start(args.port, args.open_to_world, D_TEXT, session_def);
}