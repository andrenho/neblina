#ifndef NEBLINA_CONNECTION_H
#define NEBLINA_CONNECTION_H

#include <stdint.h>
#include <stddef.h>

#include "socket.h"
#include "uthash.h"

typedef enum { D_BINARY, D_TEXT } DataType;

typedef struct {
    SOCKET         fd;          // key
    DataType       data_type;   // D_BINARY (ready as soon as it receives a byte), D_TEXT (ready when it gets a full line)
    uint8_t*       inbuf;       // input buffer
    size_t         inbuf_sz;        // buffer current size
    size_t         inbuf_rsvrd;     // memory area reserved to the buffer (malloc'd)
    bool           ready;       // ready for processing
    void*          session;     // Session provided by the service
    UT_hash_handle hh;          // UTHash handle to use this structure as a map
} ConnectionItem;


#endif //NEBLINA_CONNECTION_H
