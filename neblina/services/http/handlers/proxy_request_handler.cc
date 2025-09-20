#include "proxy_request_handler.hh"

#include "client/http_client.hh"

HttpResponse ProxyRequestHander::forward_request(std::string const& method, HttpRequest request, URLParameters const& url_parameters) const
{
    // recreate resource
    std::string original_resource = request.resource;
    size_t rel_i = std::string::npos;
    if (!url_parameters.empty()) {
        request.resource = url_parameters.at(0);
        rel_i = original_resource.find(url_parameters.at(0));
    }

    request.headers.remove("Connection");
    request.headers.remove("Proxy-Connection");
    request.headers.remove("Keep-Alive");
    request.headers.remove("Transfer-Encoding");
    request.headers.remove("Upgrade");
    request.headers.remove("TE");
    request.headers.remove("Trailer");

    // TODO - Forwarded: for=203.0.113.42; proto=https; host=example.com

    try {
        // call server
        HttpResponse r = http_client_request(destination_, port_, request);

        // rewrite Location, if needed
        if (rel_i != std::string::npos && r.status_code == 301 && r.headers.contains("Location"))
            r.headers["Location"] = original_resource.substr(0, rel_i) + *r.headers.at("Location");

        return r;

    } catch (std::exception& e) {
        return HttpResponse::error_response_html(500, e.what());
    }
}
