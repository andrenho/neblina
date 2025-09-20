#include "http_session.hh"

#include <regex>

#include "handler/custom_handler_registry.hh"
#include "handler/http_request_handler.hh"
#include "types/http_response.hh"
#include "types/http_exceptions.hh"

ConnectionStatus HttpSession::new_line(std::string_view data)
{
    ConnectionStatus r = ConnectionStatus::Open;

    try {
        // keep reading data until we receive a complete request
        current_http_request << data;

        if (current_http_request.complete()) {
            r = parse_request(current_http_request);
            current_http_request = {};
        }
    } catch (HttpException& e) {
        send_data(HttpResponse::error_response_html(e.status_code, e.what()).to_string());
        r = ConnectionStatus::Closed;
    }

    return r;
}

ConnectionStatus HttpSession::parse_request(HttpRequest const& request)
{
    // reject request if it doesn't have the "Host" header
    if (!request.headers.contains("Host"))
        throw BadRequestException();

    // find request handler
    URLParameters url_parameters;
    QueryParameters query_parameters;
    HttpRequestHandler* request_handler = find_request_handler(request, url_parameters, query_parameters);

    // execute handler for specific method
    HttpResponse response;
    switch (request.method) {
        case HttpRequest::Method::Get:       response = request_handler->get(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Post:      response = request_handler->post(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Put:       response = request_handler->put(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Delete:    response = request_handler->delete_(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Patch:     response = request_handler->patch(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Head:      response = request_handler->head(request, url_parameters, query_parameters); break;
        case HttpRequest::Method::Options:   throw NotImplementedException();
        case HttpRequest::Method::Trace:     throw NotImplementedException();
        case HttpRequest::Method::Connect:   throw NotImplementedException();
        case HttpRequest::Method::Undefined: throw InternalServerErrorException();
    }

    // compress the response, if the client accepts gzip
    if (request.headers.accepts_encoding("gzip"))
        response.compress();

    // send the response to the client
    send_data(response.to_string());

    // if client had asked to close the connection, close it
    if (request.headers.connection() == "close")
        return ConnectionStatus::Closed;

    return ConnectionStatus::Open;
}

HttpRequestHandler* HttpSession::find_request_handler(HttpRequest const& request, URLParameters& url_parameters, QueryParameters& query_parameters)
{
    // parse URI
    std::string resource = request.resource;
    std::string params;
    if (size_t i = resource.find('?') != std::string::npos) {
        params = resource.substr(i + 1);
        resource = resource.substr(0, i - 1);
    }

    // TODO - parse query parameters

    // find request handler based on the route
    for (auto const& route: routes_) {
        std::smatch m;
        if (std::regex_match(resource, m, route.regex)) {
            url_parameters.reserve(m.size());
            std::transform(m.begin() + 1, m.end(), std::back_inserter(url_parameters), [](const auto& m) { return m.str(); });
            return route.handler.get();
        }
    }

    // if request handler is not found, return a request handler that always returns 404
    return &default_request_handler;
}
