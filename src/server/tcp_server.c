#include "tcp_server.h"

#include "common.h"
#include "pool/connpool.h"
#include "poller/poller.h"
#include "socket.h"

#define MAX_EVENTS 64

static SOCKET   socket_fd;
static DataType data_type;

static const char* ERR_PRX = "TCP server error:";
static Connection* connection_set = NULL;

static void close_socket(SOCKET fd)
{
#ifdef _WIN32
    closesocket(fd);
#else
    close(fd);
#endif
}

static SOCKET get_listener_socket(int port, bool open_to_world)
{
    SOCKET listener = -1;

    // find internet address to bind
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;          // IPV4 or IPV6
    hints.ai_socktype = SOCK_STREAM;    // TCP
    hints.ai_flags = AI_PASSIVE;        // use my IP

    const char* listen_to = open_to_world ? NULL: "localhost";

    int rv;
    struct addrinfo* servinfo;
    char sport[15]; snprintf(sport, sizeof sport, "%d", port);
    if ((rv = getaddrinfo(listen_to, sport, &hints, &servinfo)) != 0)
        FATAL("%s getaddrinfo error: %s", ERR_PRX, gai_strerror(rv));

    // loop through all the results and bind to the first we can
    struct addrinfo* p;
    for(p = servinfo; p != NULL; p = p->ai_next) {

        // open socket
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener == INVALID_SOCKET)
            FATAL("%s socket error: %s", ERR_PRX, strerror(errno));

        // set socket as reusable
#ifdef _WIN32
        char yes = '1';
#else
        int yes = 1;
#endif
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == SOCKET_ERROR)
            FATAL("%s setsocket error: %s", ERR_PRX, strerror(errno));

        // bind to port
        if (bind(listener, p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
            close_socket(listener);
            continue;  // not possible, try next
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
        FATAL("%s failed to bind: %s", ERR_PRX, strerror(errno));

    if (listen(listener, 10) == SOCKET_ERROR)
        FATAL("%s listen error: %s", ERR_PRX, strerror(errno));

    LOG("listening in port %d", port);
    DBG("with fd %d", listener);

    return listener;
}

static void handle_new_connection()
{
    // accept connection
    struct sockaddr_storage remoteaddr; // Client address
    memset(&remoteaddr, 0, sizeof remoteaddr);
    socklen_t addrlen = sizeof remoteaddr;

    SOCKET new_fd = accept(socket_fd, (struct sockaddr *) &remoteaddr, &addrlen);
    if (new_fd == -1) {
        ERR("%s listen error: %s", ERR_PRX, strerror(errno));
        return;
    }

    // find connecter IP/port
    char hoststr[NI_MAXHOST] = "Unknown";
    char portstr[NI_MAXSERV] = "0";
    getnameinfo((struct sockaddr const*)(&remoteaddr), addrlen, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
    DBG("New connection from %s:%s as fd %d", hoststr, portstr, new_fd);

    // add connection to poller
    poller_add_connection(new_fd);

    // add connection to connection set
    Connection* ci = malloc(sizeof(Connection));
    *ci = (Connection) {
        .fd = new_fd,
        .data_type = data_type,
        .inbuf = malloc(DFLT_CONN_BUF_SZ),
        .inbuf_sz = 0,
        .inbuf_rsvrd = DFLT_CONN_BUF_SZ,
        .ready = false,
        .session = NULL,   // TODO - create new Session from service
    };
    HASH_ADD_INT(connection_set, fd, ci);
}

static void handle_disconnect(SOCKET fd)
{
    LOG("Disconnected from socket %d", fd);

    // remove from connection set
    Connection* ci = NULL;
    HASH_FIND_INT(connection_set, &fd, ci);
    if (ci) {
        HASH_DEL(connection_set, ci);
        free(ci->inbuf);
        free(ci);
    }

    // remove from poller
    poller_remove_connection(fd);

    // close socket
    close_socket(fd);
}

static void handle_new_data(SOCKET fd)
{
    Connection* c;
    HASH_FIND_INT(connection_set, &fd, c);
    if (!c) {
        ERR("Data to socket %d, but socket is not available", fd);
        return;
    }

    // receive data
    uint8_t buf[BUFFER_SZ];
    ssize_t sz = recv(fd, buf, sizeof buf, 0);
    if (sz < 0) {
        // client disconnected
        handle_disconnect(fd);
    } else {
        // TODO - add mutex here --\

        // add data to the input buffer
        if ((c->inbuf_sz + sz) > c->inbuf_rsvrd) {
            c->inbuf_rsvrd = c->inbuf_sz + sz;
            c->inbuf = realloc(c->inbuf, c->inbuf_rsvrd + 1);
            c->inbuf[c->inbuf_rsvrd] = '\0';
        }
        memcpy(&c->inbuf[c->inbuf_sz], buf, sz);
        c->inbuf_sz += sz;

        // check if ready to process
        if (data_type == D_BINARY || strchr((char *) c->inbuf, '\n'))
            c->ready = true;

        // TODO ------------------/

        // notify thread pool that this connection is ready to process
        if (c->ready)
            connpool_ready(fd);
    }
}

void tcp_server_start(int port, bool open_to_world, DataType data_type_)
{
    data_type = data_type_;

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        FATAL("WSAStartup() error!");
    DBG("WSAStartup() succeeded");
#endif

    // create listener
    socket_fd = get_listener_socket(port, open_to_world);

    // initialize pooler
    poller_init(socket_fd);

    // initialize thread pool
    connpool_init(8 /* TODO - get it from somewhere? */, &connection_set);

    // main loop
    while (!termination_requested) {

        PollerEvent events[MAX_EVENTS];
        size_t n_events = poller_wait(events, MAX_EVENTS);   // TODO - check for errors
        for (size_t i = 0; i < n_events; ++i) {
            switch (events[i].type) {
                case PT_NEW_CONNECTION: handle_new_connection(); break;
                case PT_NEW_DATA:       handle_new_data(events[i].fd); break;
                case PT_DISCONNECTED:   handle_disconnect(events[i].fd); break;
            }
        }
    }

    // close socket
    close_socket(socket_fd);
}