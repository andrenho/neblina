#include "connpool.h"

#include "uthash.h"

static Connection** connection_set;

void connpool_init(size_t n_threads, Connection** connection_set_)
{
    connection_set = connection_set_;
}

void connpool_ready(SOCKET fd, SessionDef* session_def, SendF send_f, void* ctx)
{
    Connection* c;
    HASH_FIND_INT(*connection_set, &fd, c);
    if (c) {
        // TODO - check if it's per line or per byte
        // TODO - possibly mark for disconnect
        if (session_def->process)
            session_def->process(c->session, c->inbuf, c->inbuf_sz, send_f, ctx);
        c->inbuf_sz = 0;
        c->ready = false;
    }
}