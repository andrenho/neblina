#include "tcp_connection.hh"

#include <cerrno>
#include <cstring>
#include <memory>
#include <unistd.h>
#include <sys/socket.h>

#include "util/log.hh"

void TCPConnection::send_data(uint8_t const* data, size_t sz)
{
    size_t pos = 0;
    while (pos < sz) {
        ssize_t const n = send(fd_, &data[pos], sz - pos, 0);
        if (n < 0) {
            ERR("Error sending data: {}", strerror(errno));
            connection_status_ = ConnectionStatus::Closed;
        }
        pos += n;
    }
}

void TCPConnection::close_connection()
{
    DBG("connection to socket {} closed by the server", fd_);
    close(fd_);
    connection_status_ = ConnectionStatus::Closed;
}

ConnectionStatus TCPConnection::handle_new_data()
{
    std::unique_ptr<char[]> buf(new char[BUFFER_SZ]);
    ssize_t n = recv(fd_, buf.get(), BUFFER_SZ, 0);

    if (n <= 0) {  // error or connection closed by client
        connection_status_ = ConnectionStatus::Closed;
    } else {       // data received from client
        std::vector<uint8_t> data(buf.get(), buf.get() + n);
        if (listener_)
            connection_status_ = listener_->new_data_available(data);
    }

    return connection_status_;
}
