#ifndef HTTP_CONNECTION_HH
#define HTTP_CONNECTION_HH

#include "http_request_handler.hh"
#include "service/tcp/tcp_connection_text.hh"
#include "types/http_request.hh"

class HttpConnection final : public TCPConnectionText {
public:
    using TCPConnectionText::TCPConnectionText;

    void new_data_available(std::string_view data) override;

private:
    HttpRequest current_http_request;
    HttpRequestHandler default_request_handler;

    HttpRequestHandler& find_request_handler(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters);

    void parse_request(HttpRequest request);
};


#endif //HTTP_ROUTER_CONNECTION_HH
