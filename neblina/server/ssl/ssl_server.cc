#include "ssl_server.hh"

#include <openssl/err.h>
#include <openssl/ssl.h>

#include "arguments.hh"
#include "ssl_connection.hh"
#include "util/exceptions/non_recoverable_exception.hh"

SSLServer::SSLServer(fs::path const& public_key, fs::path const& private_key)
{
    SSL_METHOD const* method = TLS_server_method();
    ctx_ = SSL_CTX_new(method);
    if (!ctx_) {
        ERR_print_errors_fp(stderr);
        throw NonRecoverableException("Could not initialize OpenSSL.");
    }

    if (SSL_CTX_use_certificate_file(ctx_, public_key.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        throw NonRecoverableException("Could not load public key.");
    }

    if (SSL_CTX_use_PrivateKey_file(ctx_, private_key.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
        ERR_print_errors_fp(stderr);
        throw NonRecoverableException("Could not load private key.");
    }
}

SSLServer::~SSLServer()
{
    SSL_CTX_free(ctx_);
}

std::unique_ptr<TCPConnection> SSLServer::new_connection(int fd) const
{
    return std::make_unique<SSLConnection>(fd, ctx_);
}
