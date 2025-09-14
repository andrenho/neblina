#include "http_connection.hh"

#include <regex>

#include "http_exceptions.hh"
#include "http_handler_registry.hh"
#include "types/http_response.hh"
#include "http_request_handler.hh"

HttpConnection::HttpConnection(int fd, HttpConfig const& config)
        : TCPConnectionText(fd)
{
    for (auto const& rt: config.routes) {
        try {
            routes_.emplace_back(Route {
                .regex = std::regex(rt.path),
                .handler = HttpHandlerRegistry::at(rt.handler),
            });
        } catch (std::out_of_range&) {
            throw std::runtime_error(std::format("A handler for '{}' has not been loaded.", rt.handler));
        } catch (std::regex_error&) {
            throw std::runtime_error(std::format("Path '{}' is not a valid regex expression.", rt.path));
        }
    }
}

void HttpConnection::new_data_available(std::string_view data)
{
    try {
        current_http_request << data;

        if (current_http_request.complete())
            parse_request(current_http_request);
    } catch (HttpException& e) {
        send_data(HttpResponse(e.status_code, e.what()).to_string());
        close_connection();
    }
}

void HttpConnection::parse_request(HttpRequest request)
{
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
        case HttpRequest::Method::Options:   throw MethodNotAllowedException();
        case HttpRequest::Method::Head:      response = request_handler.head(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Trace:     throw MethodNotAllowedException();
        case HttpRequest::Method::Connect:   throw MethodNotAllowedException();
        case HttpRequest::Method::Undefined: throw InternalServerErrorException();
    }
}

HttpRequestHandler& HttpConnection::find_request_handler(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters)
{
    std::string resource = request.resource();
    std::string params;
    if (size_t i = resource.find('?') == std::string::npos) {
        params = resource.substr(0, i - 1);
        resource = resource.substr(0, i - 1);
    }

    for (auto const& route: routes_) {
        // TODO
    }

    return default_request_handler;
}
