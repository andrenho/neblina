#include "tcp_server.h"

#include "common.h"
#include "os/os.h"

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  pragma comment(lib, "Ws2_32.lib")
#else
#  include <unistd.h>
#  include <arpa/inet.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  define INVALID_SOCKET -1
#  define SOCKET_ERROR -1
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
#elif __unix__
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

void tcp_server_start(int port, bool open_to_world)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        FATAL("WSAStartup() error!");
    DBG("WSAStartup() succeeded");
#endif

    socket_fd = get_listener_socket(port, open_to_world);

    while (!termination_requested) {
        os_sleep_ms(100);
    }

    // close socket
    close_socket(socket_fd);
}