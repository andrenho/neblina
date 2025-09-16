#ifndef HTTP_CONNECTION_HH
#define HTTP_CONNECTION_HH

#include <unordered_map>
#include <regex>
#include <vector>

#include "http_config.hh"
#include "http_request_handler.hh"
#include "handlers/not_found_request_handler.hh"
#include "handlers/static_dir_request_handler.hh"
#include "service/tcp/tcp_connection_text.hh"
#include "types/http_request.hh"

class HttpConnection final : public TCPConnectionText {
public:
    HttpConnection(int fd, HttpConfig const& config);

    void new_data_available(std::string_view data) override;

private:
    struct Route {
        std::regex regex;
        HttpRequestHandler* handler;
    };
    std::vector<Route> routes_;

    HttpRequest            current_http_request;
    NotFoundRequestHandler default_request_handler;
    std::unordered_map<std::string, std::unique_ptr<StaticDirRequestHandler>> static_dir_request_handlers_;

    HttpRequestHandler& find_request_handler(HttpRequest const& request, URLParameters& url_parameters, QueryParameters& query_parameters);

    void parse_request(const HttpRequest& request);
};


#endif //HTTP_CONNECTION_HH
