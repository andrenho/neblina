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
        if (session_def->process) {
            if (c->data_type == D_BINARY) {
                session_def->process(c->session, c->inbuf, c->inbuf_sz, send_f, ctx);
                c->inbuf_sz = 0;

            } else {
                char* fnd;
                while ((fnd = strstr((const char *) c->inbuf, "\r\n"))) {
                    // copy to a new buffer
                    size_t sz = fnd - (const char *) c->inbuf;
                    char buf[sz + 1];
                    memcpy(buf, (const char *) c->inbuf, sz);
                    buf[sz] = '\0';
                    session_def->process(c->session, (uint8_t const *) buf, sz, send_f, ctx);

                    // remove from input
                    memmove(c->inbuf, fnd + 2, c->inbuf_sz - (sz + 2));
                    c->inbuf_sz -= (sz + 2);
                    c->inbuf[c->inbuf_sz] = '\0';
                }
            }

        }
        c->ready = false;
    }
}