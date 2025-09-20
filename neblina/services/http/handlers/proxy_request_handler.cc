#include "proxy_request_handler.hh"

#include "client/http_client.hh"

HttpResponse ProxyRequestHander::forward_request(std::string const& method, HttpRequest request, URLParameters const& url_parameters) const
{
    if (!url_parameters.empty())
        request.resource = url_parameters.at(0);
    request.headers.remove("Connection");
    request.headers.remove("Proxy-Connection");
    request.headers.remove("Keep-Alive");
    request.headers.remove("Transfer-Encoding");
    request.headers.remove("Upgrade");
    request.headers.remove("TE");
    request.headers.remove("Trailer");

    // TODO - Forwarded: for=203.0.113.42; proto=https; host=example.com

    try {
        return http_client_request(destination_, port_, request);
    } catch (std::exception& e) {
        return HttpResponse::error_response_html(500, e.what());
    }
}
