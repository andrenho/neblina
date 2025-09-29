#include "parrot.h"

#include "common.h"
#include "server/tcp_server.h"

/*  TODO - process line by line
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

static void parrot_process(void* session, uint8_t const* data, size_t data_sz, SendF send_f, SOCKET fd)
{
    // TODO - process line by line
    (void) session;
    send_f(fd, data, data_sz);
    // send_f(fd, (uint8_t const *) "\r\n", 2);
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