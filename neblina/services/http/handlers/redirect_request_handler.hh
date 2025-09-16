#ifndef REDIRECT_REQUEST_HANDLER_HH
#define REDIRECT_REQUEST_HANDLER_HH

#include "../http_request_handler.hh"

class RedirectRequestHandler : public HttpRequestHandler {
public:
    RedirectRequestHandler(std::string const& new_location) : new_location_(new_location) {}

    HttpResponse get(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) const override
    {
        return HttpResponse::redirect_response(new_location_);
    }

    HttpResponse post(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override
    {
        return HttpResponse::redirect_response(new_location_);
    }

    HttpResponse patch(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override
    {
        return HttpResponse::redirect_response(new_location_);
    }

    HttpResponse put(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override
    {
        return HttpResponse::redirect_response(new_location_);
    }

    HttpResponse delete_(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override
    {
        return HttpResponse::redirect_response(new_location_);
    }

private:
    std::string new_location_;
};

#endif //REDIRECT_REQUEST_HANDLER_HH
