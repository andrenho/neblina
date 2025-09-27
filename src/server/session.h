#ifndef NEBLINA_SESSION_FACTORY_H
#define NEBLINA_SESSION_FACTORY_H

#include <stdint.h>
#include <stddef.h>

typedef bool(SendF)(uint8_t const* data, size_t sz, void* ctx);

typedef void Session;

typedef struct {
    Session* (*create_session)();
    void     (*process_session)(Session* session, uint8_t const* data, size_t data_sz, SendF send_f, void* ctx);
    void     (*destroy_session)(Session* session);
} SessionCallbacks;

#endif //NEBLINA_SESSION_FACTORY_H
