#ifndef SSL_CONNECTION_HH
#define SSL_CONNECTION_HH

#include <openssl/types.h>

#include "server/tcp/tcp_connection.hh"

class SSLConnection : public TCPConnection {
public:
    explicit SSLConnection(int fd, SSL_CTX* ctx);
    ~SSLConnection() override;

    ConnectionStatus handle_new_data() override;
    void send_data(uint8_t const* data, size_t sz) override;

private:
    SSL* ssl_ = nullptr;
};

#endif //SSL_CONNECTION_HH
