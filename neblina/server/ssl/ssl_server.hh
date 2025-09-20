#ifndef SSL_SERVER_HH
#define SSL_SERVER_HH

#include <openssl/types.h>

#include "../tcp/tcp_server.hh"
#include "util/filesystem.hh"

class SSLServer : public TCPServer {
public:
    SSLServer(fs::path const& public_key, fs::path const& private_key);
    ~SSLServer() override;

protected:
    std::unique_ptr<TCPConnection> new_connection(int fd, std::string const& host, std::string const& port) const override;

private:
    SSL_CTX* ctx_ = nullptr;
};

#endif //SSL_SERVER_HH
