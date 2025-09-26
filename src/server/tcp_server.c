#include "tcp_server.h"

#include "common.h"
#include "os/os.h"
#include "poller/poller.h"

#define MAX_EVENTS 64

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  pragma comment(lib, "Ws2_32.lib")
#else
#  define _DARWIN_C_SOURCE
#  include <unistd.h>
#  include <arpa/inet.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  define INVALID_SOCKET (-1)
#  define SOCKET_ERROR (-1)
   typedef int SOCKET;
   typedef struct sockaddr_in SOCKADDR_IN;
   typedef struct sockaddr SOCKADDR;
#endif

static SOCKET socket_fd;

static const char* ERR_PRX = "TCP server error:";

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
	memset(&servinfo, 0, sizeof(servinfo));
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
    struct sockaddr_storage remoteaddr; // Client address
    memset(&remoteaddr, 0, sizeof remoteaddr);
    socklen_t addrlen = sizeof remoteaddr;

    SOCKET new_fd = accept(socket_fd, (struct sockaddr *) &remoteaddr, &addrlen);
    if (new_fd == -1) {
        ERR("%s listen error: %s", ERR_PRX, strerror(errno));
        return;
    }

    char hoststr[NI_MAXHOST] = "Unknown";
    char portstr[NI_MAXSERV] = "0";

    getnameinfo((struct sockaddr const*)(&remoteaddr), addrlen, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
    DBG("New connection from %s:%s as fd %d", hoststr, portstr, new_fd);

    // TODO - create new connection

    poller_add_connection(new_fd);
}

static void handle_new_data(SOCKET fd)
{
    LOG("New data");
    uint8_t buf[255];
    recv(fd, buf, sizeof buf, 0);
}

static void handle_disconnect(SOCKET fd)
{
    LOG("Disconnected");
    poller_remove_connection(fd);
}

void tcp_server_start(int port, bool open_to_world, CreateConnectionF cf, ProcessConnectionF pf)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        FATAL("WSAStartup() error!");
    DBG("WSAStartup() succeeded");
#endif

    socket_fd = get_listener_socket(port, open_to_world);
    poller_init(socket_fd);   // TODO - check for errors

    while (!termination_requested) {

        PollerEvent events[MAX_EVENTS];
        size_t n_events = poller_wait(events, MAX_EVENTS);   // TODO - check for errors
        for (size_t i = 0; i < n_events; ++i) {
            if (events[i].type == PT_NEW_CONNECTION) {
                handle_new_connection();
            } else if (events[i].type == PT_NEW_DATA) {
                handle_new_data(events[i].fd);
            } else if (events[i].type == PT_DISCONNECTED) {
                handle_disconnect(events[i].fd);
            }
        }
    }

    // close socket
    close_socket(socket_fd);
}