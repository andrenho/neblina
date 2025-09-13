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
#include "service/tcp/tcp_service.hh"
#include "util/log.hh"

using namespace std::string_literals;

TCPServer::TCPServer(TCPService* service)
    : listener_(get_listener_socket()), service_(service)
{
}

TCPServer::~TCPServer()
{
    close(listener_);
}

int TCPServer::get_listener_socket()
{
    int listener = -1;

    // find internet address to bind
    addrinfo hints {};
    hints.ai_family = AF_INET;          // IPV4 or IPV6
    hints.ai_socktype = SOCK_STREAM;    // TCP
    hints.ai_flags = AI_PASSIVE;        // use my IP

    int rv;
    addrinfo* servinfo;
    if ((rv = getaddrinfo(nullptr, std::to_string(args().port).c_str(), &hints, &servinfo)) != 0)
        throw std::runtime_error("getaddrinfo error: "s + gai_strerror(rv));

    // loop through all the results and bind to the first we can
    addrinfo* p;
    for(p = servinfo; p != nullptr; p = p->ai_next) {

        // open socket
        if ((listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            throw std::runtime_error("socket error: "s + strerror(errno));

        // set socket as reusable
        int yes = 1;
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
            throw std::runtime_error("setsocket error: "s + strerror(errno));

        // bind to port
        if (bind(listener, p->ai_addr, p->ai_addrlen) == -1) {
            close(listener);
            continue;  // not possible, try next
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == nullptr)
        throw std::runtime_error("failed to bind: "s + strerror(errno));

    if (listen(listener, 10) == -1)
        throw std::runtime_error("setsocket error: "s + strerror(errno));

    log("listening in port {}", args().port);

    return listener;
}

void TCPServer::run()
{
    std::vector<pollfd> poll_fds(1);

    poll_fds.at(0).fd = listener_;
    poll_fds.at(0).events = POLLIN;

    while (server_running_) {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), 20);
        if (poll_count == -1)
            throw std::runtime_error("poll error: "s + strerror(errno));

        // Run through connections looking for data to read
        for (auto& pollfd: poll_fds) {
            if (pollfd.revents & (POLLIN | POLLHUP)) {
                if (pollfd.fd == listener_)
                    handle_new_connection(poll_fds);
                else
                    handle_new_data(pollfd, poll_fds);
            }
        }
    }
}

void TCPServer::handle_new_connection(std::vector<pollfd>& poll_fds)
{
    sockaddr_storage remoteaddr{}; // Client address
    socklen_t addrlen = sizeof remoteaddr;

    int new_fd = accept(listener_, (sockaddr *) &remoteaddr, &addrlen);
    if (new_fd == -1)
        throw std::runtime_error("accept error: "s + strerror(errno));

    poll_fds.push_back({ .fd = new_fd, .events = POLLIN, .revents = 0 });
    connections_[new_fd] = service_->new_connection(new_fd);
}

void TCPServer::handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds)
{
    std::unique_ptr<char[]> buf(new char[BUFFER_SZ]);
    ssize_t n = recv(pfd.fd, buf.get(), BUFFER_SZ, 0);

    if (n <= 0) {  // error or connection closed by client
        close(pfd.fd);
        poll_fds.erase(
            std::remove_if(poll_fds.begin(), poll_fds.end(), [&](pollfd const& p) { return p.fd == pfd.fd; }),
            poll_fds.end());
        connections_.erase(pfd.fd);
    } else {   // data received from client
        std::vector<uint8_t> data(buf.get(), buf.get() + n);
        auto const& connection = connections_.at(pfd.fd);
        connection->new_data_available(data);
        if (connection->connection_status() == ConnectionStatus::Closed)
            connections_.erase(pfd.fd);
    }
}

void TCPServer::send_data(std::vector<uint8_t> const& data, int fd)
{
    size_t pos = 0;
    while (pos < data.size()) {
        int n = send(fd, &data.data()[pos], data.size() - pos, 0);
        if (n < 0)
            throw std::runtime_error("Error sending data: "s + strerror(errno));
        pos += n;
    }
}

void TCPServer::send_data(const char* data, int fd)
{
    std::vector<uint8_t> v(data, data + strlen(data));
    send_data(v, fd);
}

void TCPServer::close_connection(int fd)
{
    close(fd);
}
