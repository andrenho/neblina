#include "connpool.h"

#include "uthash.h"

static Connection** connection_set;

void connpool_init(size_t n_threads, Connection** connection_set_)
{
    (void) n_threads;
    connection_set = connection_set_;
}

/*
void coonpool_process_connection(Connection *c, const SessionCallbacks *session_def, SendF send_f, void *ctx) {
    if (session_def->process_session) {
        if (c->data_type == D_BINARY) {
            session_def->process_session(c->session, c->inbuf, c->inbuf_sz, send_f, ctx);
            c->inbuf_sz = 0;

        } else {
            char* fnd;
            while ((fnd = strstr((const char *) c->inbuf, "\r\n"))) {
                // copy to a new buffer
                size_t sz = fnd - (const char *) c->inbuf;
                char* buf = calloc(1, sz + 1);
                memcpy(buf, (const char *) c->inbuf, sz);
                session_def->process_session(c->session, (uint8_t const *) buf, sz, send_f, ctx);
                free(buf);

                // remove from input
                memmove(c->inbuf, fnd + 2, c->inbuf_sz - (sz + 2));
                c->inbuf_sz -= (sz + 2);
                c->inbuf[c->inbuf_sz] = '\0';
            }
        }

    }
    c->ready = false;
}
 */

void connpool_add_task(SOCKET fd, uint8_t* data, size_t data_sz, ProcessSessionF process_cb, void* session, SendF send_f)
{
    process_cb(session, data, data_sz, send_f, fd);
    free(data);
}

void connpoll_finalize()
{
}
