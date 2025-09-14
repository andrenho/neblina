#include "http_connection.hh"

#include "http_exceptions.hh"
#include "types/http_response.hh"
#include "http_request_handler.hh"

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
            break;
    }
}

HttpRequestHandler& HttpConnection::find_request_handler(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters)
{
    return default_request_handler;
}
