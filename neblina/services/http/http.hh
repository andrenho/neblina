#ifndef HTTP_HH
#define HTTP_HH

#include "service/tcp/tcp_connection_text.hh"
#include "service/tcp/tcp_service.hh"
#include "util/http/http_request.hh"

class HttpConnection final : public TCPConnectionText {
public:
    using TCPConnectionText::TCPConnectionText;

    void new_data_available(std::string_view data) override;

private:
    HttpRequest current_http_request;
};

class Http final : public TCPService {
public:
    SERVICE_NAME = "http";

    std::unique_ptr<TCPConnection> new_connection(int fd) const override {
        return std::make_unique<HttpConnection>(fd);
    }
};

#endif //HTTP_HH
