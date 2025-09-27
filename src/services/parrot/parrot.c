#include "parrot.h"

#include "common.h"
#include "server/tcp_server.h"

static Session* parrot_init()
{
    LOG("New PARROT session");
    return NULL;
}

static void parrot_process(Session* session, uint8_t const* data, size_t data_sz, SendF send_f, void* ctx)
{
    (void) session;
    send_f(data, data_sz, ctx);
    send_f((uint8_t const *) "\r\n", 2, ctx);
}

static void parrot_finalize(Session* session)
{
    (void) session;
    LOG("PARROT session finalized");
}

void start_parrot()
{
    SessionDef session_def = {
            .init = parrot_init,
            .process = parrot_process,
            .finalize = parrot_finalize,
    };
    tcp_server_start(args.port, args.open_to_world, D_TEXT, session_def);
}