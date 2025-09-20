#ifndef PROXY_REQUEST_HANDER_HH
#define PROXY_REQUEST_HANDER_HH

#include "../handler/http_request_handler.hh"

class ProxyRequestHander : public HttpRequestHandler {
public:
    explicit ProxyRequestHander(std::string const& destination) : destination_(destination) {}

    HttpResponse get(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) const override { return forward_request("GET", request); }

    HttpResponse post(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("POST", request); }

    HttpResponse patch(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("PATCH", request); }

    HttpResponse put(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("PUT", request); }

    HttpResponse delete_(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("DELETE", request); }

    HttpResponse head(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) const override { return forward_request("HEAD", request); }

private:
    std::string destination_;

    HttpResponse forward_request(std::string const& method, HttpRequest const& request) const;
};



#endif //PROXY_REQUEST_HANDER_HH
