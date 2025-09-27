#include "parrot.h"

#include "common.h"
#include "server/tcp_server.h"

static Session* parrot_init()
{
    LOG("New PARROT session");
    return NULL;
}

bool parrot_process(Session* session, uint8_t* data, size_t data_sz, SendF send_f, void* ctx)
{
    (void) session;
    return send_f(data, data_sz, ctx);
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