#include "http_connection.hh"

#include <regex>

#include "http_exceptions.hh"
#include "http_handler_registry.hh"
#include "types/http_response.hh"
#include "http_request_handler.hh"
#include "util/string.hh"

HttpConnection::HttpConnection(int fd, HttpConfig const& config)
        : TCPConnectionText(fd)
{
    // create list of request handers

    for (auto const& rt: config.routes) {
        try {
            HttpRequestHandler* handler = nullptr;
            if (rt.handler) {
                handler = HttpHandlerRegistry::at(*rt.handler);
            } else if (rt.serve_static_dir) {
                static_dir_request_handlers_[*rt.serve_static_dir] = std::make_unique<StaticDirRequestHandler>(*rt.serve_static_dir);
                handler = static_dir_request_handlers_.at(*rt.serve_static_dir).get();
            }
            if (handler) {
                routes_.emplace_back(Route {
                    .regex = std::regex(rt.path),
                    .handler = handler,
                });
            }
        } catch (std::out_of_range&) {
            throw std::runtime_error(std::format("Handler '{}' is not available.", *rt.handler));
        } catch (std::regex_error&) {
            throw std::runtime_error(std::format("Path '{}' is not a valid regex expression.", rt.path));
        }
    }
}

void HttpConnection::new_data_available(std::string_view data)
{
    try {
        current_http_request << data;

        if (current_http_request.complete()) {
            parse_request(current_http_request);
            current_http_request = {};
        }
    } catch (HttpException& e) {
        send_data(HttpResponse::error_response_html(e.status_code, e.what()).to_string());
        close_connection();
    }
}

void HttpConnection::parse_request(HttpRequest request)
{
    if (!request.headers().contains("Host"))
        throw BadRequestException();

    URLParameters url_parameters;
    QueryParameters query_parameters;
    HttpRequestHandler& request_handler = find_request_handler(request, url_parameters, query_parameters);

    HttpResponse response;
    switch (request.method()) {
        case HttpRequest::Method::Get:       response = request_handler.get(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Post:      response = request_handler.post(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Put:       response = request_handler.put(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Delete:    response = request_handler.delete_(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Patch:     response = request_handler.patch(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Options:   throw NotImplementedException();
        case HttpRequest::Method::Head:      response = request_handler.head(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Trace:     throw NotImplementedException();
        case HttpRequest::Method::Connect:   throw NotImplementedException();
        case HttpRequest::Method::Undefined: throw InternalServerErrorException();
    }

    send_data(response.to_string());

    auto close_conn = request.headers().at("Connection");
    if (close_conn && to_lower(*close_conn) == "close")
        close_connection();
}

HttpRequestHandler& HttpConnection::find_request_handler(HttpRequest const& request, URLParameters& url_parameters, QueryParameters& query_parameters)
{
    std::string resource = request.resource();
    std::string params;
    if (size_t i = resource.find('?') != std::string::npos) {
        params = resource.substr(i + 1);
        resource = resource.substr(0, i - 1);
    }

    for (auto const& route: routes_) {
        std::smatch m;
        if (std::regex_match(resource, m, route.regex)) {
            url_parameters.reserve(m.size());
            std::transform(m.begin() + 1, m.end(), std::back_inserter(url_parameters), [](const auto& m) { return m.str(); });
            return *route.handler;
        }
    }

    return default_request_handler;
}
