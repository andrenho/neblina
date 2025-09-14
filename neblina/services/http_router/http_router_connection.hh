#ifndef HTTP_ROUTER_CONNECTION_HH
#define HTTP_ROUTER_CONNECTION_HH

#include "service/tcp/tcp_connection_text.hh"
#include "util/http/http_request.hh"

class HttpRouterConnection final : public TCPConnectionText {
public:
    using TCPConnectionText::TCPConnectionText;

    void new_data_available(std::string_view data) override;

private:
    HttpRequest current_http_request;

    void parse_request(HttpRequest request);
};


#endif //HTTP_ROUTER_CONNECTION_HH
