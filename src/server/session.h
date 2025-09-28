#ifndef NEBLINA_SESSION_FACTORY_H
#define NEBLINA_SESSION_FACTORY_H

#include <stdint.h>
#include <stddef.h>

typedef bool(SendF)(SOCKET fd, uint8_t const* data, size_t sz);

typedef void*(*CreateSessionF)();
typedef void (*ProcessSessionF)(void* session, uint8_t const* data, size_t data_sz, SendF send_f, SOCKET fd);
typedef void*(*DestroySessionF)(void* session);

typedef struct {
    CreateSessionF  create_session;
    ProcessSessionF process_session;
    DestroySessionF destroy_session;
} SessionCallbacks;

#endif //NEBLINA_SESSION_FACTORY_H
