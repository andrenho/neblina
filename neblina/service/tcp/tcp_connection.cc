#include "tcp_connection.hh"

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

#include "util/log.hh"

void TCPConnection::send_data(uint8_t const* data, size_t sz)
{
    size_t pos = 0;
    while (pos < sz) {
        ssize_t const n = send(fd_, &data[pos], sz - pos, 0);
        if (n < 0) {
            err("Error sending data: {}", strerror(errno));
            connection_status_ = ConnectionStatus::Closed;
        }
        pos += n;
    }
}

void TCPConnection::close_connection()
{
    close(fd_);
    connection_status_ = ConnectionStatus::Closed;
}
