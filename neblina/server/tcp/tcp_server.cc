#include "tcp_server.hh"

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>

#include "arguments.hh"
#include "service/communication/communication_service.hh"
#include "tcp_connection.hh"
#include "util/log.hh"

using namespace std::string_literals;

static const std::string ERR_PRX = "TCP server error: ";

TCPServer::TCPServer()
    : socket_fd_(get_listener_socket())
{
    poll_fds.at(0).fd = socket_fd_;
    poll_fds.at(0).events = POLLIN;
}

TCPServer::~TCPServer()
{
    close(socket_fd_);
}

int TCPServer::get_listener_socket()
{
    int listener = -1;

    // find internet address to bind
    addrinfo hints {};
    hints.ai_family = AF_INET;          // IPV4 or IPV6
    hints.ai_socktype = SOCK_STREAM;    // TCP
    hints.ai_flags = AI_PASSIVE;        // use my IP

    const char* listen_to = args().open_to_world ? nullptr : "localhost";

    int rv;
    addrinfo* servinfo;
    if ((rv = getaddrinfo(listen_to, std::to_string(args().port).c_str(), &hints, &servinfo)) != 0)
        throw std::runtime_error(ERR_PRX + "getaddrinfo error: "s + gai_strerror(rv));

    // loop through all the results and bind to the first we can
    addrinfo* p;
    for(p = servinfo; p != nullptr; p = p->ai_next) {

        // open socket
        if ((listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            throw std::runtime_error(ERR_PRX + "socket error: "s + strerror(errno));

        // set socket as reusable
        int yes = 1;
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            throw std::runtime_error(ERR_PRX + "setsocket error: "s + strerror(errno));

        // bind to port
        if (bind(listener, p->ai_addr, p->ai_addrlen) == -1) {
            close(listener);
            continue;  // not possible, try next
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == nullptr)
        throw std::runtime_error(ERR_PRX + "failed to bind: "s + strerror(errno));

    if (listen(listener, 10) == -1)
        throw std::runtime_error(ERR_PRX + "setsocket error: "s + strerror(errno));

    LOG("listening in port {}", args().port);
    DBG("with fd {}", listener);

    return listener;
}

void TCPServer::iteration()
{
    int poll_count = poll(poll_fds.data(), poll_fds.size(), 20);
    if (poll_count == -1)
        throw std::runtime_error(ERR_PRX + "poll error: "s + strerror(errno));

    // Run through connections looking for data to read
    for (auto& pollfd: poll_fds) {
        if (pollfd.revents & (POLLIN | POLLHUP)) {
            if (pollfd.fd == socket_fd_)
                handle_new_connection(poll_fds);
            else
                handle_new_data(pollfd, poll_fds);
        }
    }
}

void TCPServer::handle_new_connection(std::vector<pollfd>& poll_fds)
{
    sockaddr_storage remoteaddr{}; // Client address
    socklen_t addrlen = sizeof remoteaddr;

    int new_fd = accept(socket_fd_, (sockaddr *) &remoteaddr, &addrlen);
    if (new_fd == -1)
        throw std::runtime_error(ERR_PRX + "accept error: "s + strerror(errno));

    char hoststr[NI_MAXHOST] = "Unknown";
    char portstr[NI_MAXSERV] = "0";

    getnameinfo((sockaddr const*)(&remoteaddr), addrlen, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
    DBG("New connection from {}:{} as fd {}", hoststr, portstr, new_fd);

    poll_fds.push_back({ .fd = new_fd, .events = POLLIN, .revents = 0 });
    connections_[new_fd] = new_connection(new_fd);
    if (listener_)
        listener_->new_connection(connections_.at(new_fd).get());
}

std::unique_ptr<TCPConnection> TCPServer::new_connection(int fd) const
{
    return std::make_unique<TCPConnection>(fd);
}

void TCPServer::handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds)
{
    ConnectionStatus status = connections_.at(pfd.fd)->handle_new_data();
    if (status == ConnectionStatus::Closed) {
        DBG("connection from fd {} closed by the client ({})", pfd.fd, strerror(errno));
        close(pfd.fd);
        if (listener_)
            listener_->new_connection(connections_.at(pfd.fd).get());
        std::ranges::remove_if(poll_fds, [&](pollfd const& p) { return p.fd == pfd.fd; }),
        connections_.erase(pfd.fd);
    }
}

