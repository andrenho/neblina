#ifndef PROXY_REQUEST_HANDER_HH
#define PROXY_REQUEST_HANDER_HH

#include "../handler/http_request_handler.hh"

class ProxyRequestHander : public HttpRequestHandler {
public:
    explicit ProxyRequestHander(std::string const& destination, int port) : destination_(destination), port_(port) {}

    [[nodiscard]] HttpResponse get(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) const override { return forward_request("GET", request, url_parameters); }

    HttpResponse post(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("POST", request, url_parameters); }

    HttpResponse patch(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("PATCH", request, url_parameters); }

    HttpResponse put(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("PUT", request, url_parameters); }

    HttpResponse delete_(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override { return forward_request("DELETE", request, url_parameters); }

    [[nodiscard]] HttpResponse head(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) const override { return forward_request("HEAD", request, url_parameters); }

private:
    std::string destination_;
    int         port_;

    [[nodiscard]] HttpResponse forward_request(std::string const& method, HttpRequest request, URLParameters const& url_parameters) const;
};



#endif //PROXY_REQUEST_HANDER_HH
