#include "connpool.h"

#include <pthread.h>

#include "uthash.h"
#include "os/os.h"

typedef struct {
    bool            in_execution;
    uint8_t         thread_n;
    pthread_mutex_t mutex;
} ThreadContext;

static Connection**   connection_set;
static size_t         n_threads;
static pthread_t*     thread;
static ThreadContext* thread_ctx;

static void* thread_procedure(void* ptr)
{
    ThreadContext* ctx = ptr;

    DBG("Thread %d initialized", ctx->thread_n);

    for (;;) {
        pthread_mutex_lock(&ctx->mutex);

        os_sleep_ms(700);
        // DBG("Thread %d executed", ctx->thread_n);

        bool in_execution = ctx->in_execution;
        pthread_mutex_unlock(&ctx->mutex);
        if (!in_execution)
            break;
    }

    DBG("Thread %d finalized", ctx->thread_n);

    return NULL;
}

void connpool_init(size_t n_threads_, Connection** connection_set_)
{
    connection_set = connection_set_;
    n_threads = n_threads_;

    thread = calloc(n_threads, sizeof(pthread_t));
    thread_ctx = calloc(n_threads, sizeof(ThreadContext));
    for (size_t i = 0; i < n_threads; ++i) {
        thread_ctx[i].in_execution = true;
        thread_ctx[i].thread_n = i;
        pthread_mutex_init(&thread_ctx[i].mutex, NULL);
        pthread_create(&thread[i], NULL, thread_procedure, &thread_ctx[i]);
    }
}

void coonpool_process_connection(Connection *c, const SessionCallbacks *session_def, SendF send_f, void *ctx)
{
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

void connpool_ready(SOCKET fd, SessionCallbacks* session_def, SendF send_f, void* ctx)
{
    Connection* c;
    HASH_FIND_INT(*connection_set, &fd, c);
    if (c)
        coonpool_process_connection(c, session_def, send_f, ctx);
}

void connpoll_finalize()
{
    DBG("Requesting thread finalization");

    for (size_t i = 0; i < n_threads; ++i) {
        pthread_mutex_lock(&thread_ctx[i].mutex);
        thread_ctx[i].in_execution = false;
        pthread_mutex_unlock(&thread_ctx[i].mutex);
    }

    DBG("Joining threads");

    for (size_t i = 0; i < n_threads; ++i) {
        pthread_join(thread[i], NULL);
        pthread_mutex_destroy(&thread_ctx[i].mutex);
    }

    free(thread_ctx);
    free(thread);

    DBG("Thread pool finalized");
}
