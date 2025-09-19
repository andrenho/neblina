#include "ssl_connection.hh"

#include <memory>
#include <stdexcept>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include "util/log.hh"

SSLConnection::SSLConnection(int fd, SSL_CTX* ctx)
    : TCPConnection(fd), ssl_(SSL_new(ctx))
{
    SSL_set_fd(ssl_, fd);
    if (SSL_accept(ssl_) <= 0) {
        ERR_print_errors_fp(stderr);
        ERR("Could not open a SSL connection");
        connection_status_ = ConnectionStatus::Closed;
    }
}

SSLConnection::~SSLConnection()
{
    SSL_shutdown(ssl_);
    SSL_free(ssl_);
}

ConnectionStatus SSLConnection::handle_new_data()
{
    std::unique_ptr<char[]> buf(new char[BUFFER_SZ]);
    ssize_t n = SSL_read(ssl_, buf.get(), BUFFER_SZ);

    if (n <= 0) {  // error or connection closed by client
        connection_status_ = ConnectionStatus::Closed;
    } else {       // data received from client
        std::vector<uint8_t> data(buf.get(), buf.get() + n);
        if (listener_)
            connection_status_ = listener_->new_data_available(data);
    }

    return connection_status_;
}

void SSLConnection::send_data(uint8_t const* data, size_t sz)
{
    size_t pos = 0;
    while (pos < sz) {
        ssize_t const n = SSL_write(ssl_, data, sz);
        if (n < 0) {
            ERR("Error sending data: {}", strerror(errno));
            connection_status_ = ConnectionStatus::Closed;
        }
        pos += n;
    }
}
