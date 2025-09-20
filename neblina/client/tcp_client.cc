#include "tcp_client.hh"

#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <stdexcept>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "util/log.hh"
#include "util/string.hh"

static const std::string ERR_PRX = "TCP client error: ";

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &((sockaddr_in*)sa)->sin_addr;
    }
    return &((sockaddr_in6*)sa)->sin6_addr;
}

TCPClient::TCPClient(std::string const& destination, int port)
{
    addrinfo hints {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv;
    addrinfo *servinfo;
    if ((rv = getaddrinfo(destination.c_str(), std::to_string(port).c_str(), &hints, &servinfo)) != 0)
        throw std::runtime_error(ERR_PRX + "getaddrinfo error: "s + gai_strerror(rv));

    addrinfo* p;
    char s[INET6_ADDRSTRLEN];
    for(p = servinfo; p != nullptr; p = p->ai_next) {
        if ((fd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            DBG("{}socket error: {}", ERR_PRX, strerror(errno));
            continue;
        }

        inet_ntop(p->ai_family, get_in_addr(p->ai_addr), s, sizeof s);
        DBG("{}attempting connection to {}", ERR_PRX, std::string(s));

        if (connect(fd_, p->ai_addr, p->ai_addrlen) == -1) {
            DBG("{}connect error: {}", ERR_PRX, strerror(errno));
            close(fd_);
            continue;
        }

        break;
    }

    if (p == nullptr)
        throw std::runtime_error("Failed to connect");

    inet_ntop(p->ai_family, get_in_addr(p->ai_addr), s, sizeof s);
    LOG("{}connected to {}", ERR_PRX, std::string(s));

    freeaddrinfo(servinfo);
}

std::string TCPClient::recv(size_t nbytes)
{
    // all characters are in buffer
    if (buffer_.size() > nbytes) {
        std::string r = buffer_.substr(0, nbytes);
        buffer_ = buffer_.substr(nbytes);
        return r;
    }

    // buffer not large enough, so we read more bytes from the socket
    char buf[nbytes];
    ssize_t n = ::recv(fd_, buf, nbytes, 0);
    if (n == 0) {
        close(fd_);
        connection_closed_ = true;
        std::string r = buffer_;
        buffer_.clear();
        return buffer_;
    } else if (n < 0) {
        close(fd_);
        connection_closed_ = true;
        throw std::runtime_error(ERR_PRX + "error reading data: " + strerror(errno));
    } else {
        std::string r = buffer_ + std::string(buf, buf + n);
        buffer_.clear();
        return r;
    }
}

std::string TCPClient::recv_line(std::string const& separator)
{
    for (;;) {
        size_t i = buffer_.find(separator);
        if (i != std::string::npos) {  // separator found
            std::string r = buffer_.substr(0, i);
            buffer_ = buffer_.substr(i + separator.size());
            return r;
        }
        buffer_ += recv(1024);
    }
}

void TCPClient::send(std::string const& data)
{
    size_t pos = 0;
    while (pos < data.size()) {
        ssize_t const n = ::send(fd_, &data[pos], data.size() - pos, 0);
        if (n < 0) {
            close(fd_);
            connection_closed_ = true;
            throw std::runtime_error(ERR_PRX + "error sending data: " + strerror(errno));
        }
        pos += n;
    }
}
