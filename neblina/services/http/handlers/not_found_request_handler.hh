#ifndef NOT_FOUND_REQUEST_HANDLER_HH
#define NOT_FOUND_REQUEST_HANDLER_HH

#include "../http_request_handler.hh"

class NotFoundRequestHandler : public HttpRequestHandler {
public:
    HttpResponse get(const HttpRequest& request, const URLParameters& url_parameters,
        const QueryParameters& query_parameters) const override { throw NotFoundException(); }
    HttpResponse post(const HttpRequest& request, const URLParameters& url_parameters,
        const QueryParameters& query_parameters) override { throw NotFoundException(); }
    HttpResponse patch(const HttpRequest& request, const URLParameters& url_parameters,
        const QueryParameters& query_parameters) override { throw NotFoundException(); }
    HttpResponse put(const HttpRequest& request, const URLParameters& url_parameters,
        const QueryParameters& query_parameters) override { throw NotFoundException(); }
    HttpResponse delete_(const HttpRequest& request, const URLParameters& url_parameters,
        const QueryParameters& query_parameters) override { throw NotFoundException(); }
};

#endif //NOT_FOUND_REQUEST_HANDLER_HH
