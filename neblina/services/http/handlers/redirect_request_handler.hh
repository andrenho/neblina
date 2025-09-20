#ifndef REDIRECT_REQUEST_HANDLER_HH
#define REDIRECT_REQUEST_HANDLER_HH

#include "../handler/http_request_handler.hh"

class RedirectRequestHandler : public HttpRequestHandler {
public:
    explicit RedirectRequestHandler(std::string const& new_location) : new_location_(new_location) {}

    [[nodiscard]] HttpResponse get(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) const override
    {
        return redirect_response(url_parameters);
    }

    HttpResponse post(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override
    {
        return redirect_response(url_parameters);
    }

    HttpResponse patch(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override
    {
        return redirect_response(url_parameters);
    }

    HttpResponse put(HttpRequest const& request, URLParameters const& url_parameters,
        QueryParameters const& query_parameters) override
    {
        return redirect_response(url_parameters);
    }

    HttpResponse delete_(HttpRequest const& request, URLParameters const& url_parameters,
                         QueryParameters const& query_parameters) override
    {
        return redirect_response(url_parameters);
    }

    [[nodiscard]] HttpResponse redirect_response(URLParameters const& url_parameters) const
    {
        if (!url_parameters.empty())
            return HttpResponse::redirect_response(new_location_ + url_parameters.at(0));
        else
            return HttpResponse::redirect_response(new_location_);
    }

private:
    std::string new_location_;
};

#endif //REDIRECT_REQUEST_HANDLER_HH
