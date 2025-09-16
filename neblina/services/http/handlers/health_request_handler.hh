#ifndef HEALTH_REQUEST_HANDLER_HH
#define HEALTH_REQUEST_HANDLER_HH

#include "../http_request_handler.hh"

class HealthRequestHandler : public HttpRequestHandler {
public:
    REQUEST_HANDLER_NAME = "health-reqhandler";

    HttpResponse get(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters) const override
    {
        return HttpResponse(200, ContentType::TXT, "Success\r\n");
    }
};

#endif //HEALTH_REQUEST_HANDLER_HH
