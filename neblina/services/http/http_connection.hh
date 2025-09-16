#ifndef HTTP_CONNECTION_HH
#define HTTP_CONNECTION_HH

#include <vector>

#include "http_config.hh"
#include "http_request_handler.hh"
#include "http_routes.hh"
#include "handlers/not_found_request_handler.hh"
#include "service/tcp/tcp_connection_text.hh"
#include "types/http_request.hh"

class HttpConnection final : public TCPConnectionText {
public:
    HttpConnection(int fd, std::vector<HttpRoute> const& routes) : TCPConnectionText(fd), routes_(routes) {}

    void new_data_available(std::string_view data) override;

private:
    std::vector<HttpRoute> const& routes_;
    HttpRequest                   current_http_request;
    NotFoundRequestHandler        default_request_handler;

    HttpRequestHandler* find_request_handler(HttpRequest const& request, URLParameters& url_parameters, QueryParameters& query_parameters);

    void parse_request(const HttpRequest& request);
};


#endif //HTTP_CONNECTION_HH
