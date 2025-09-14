#ifndef HEALTH_REQUEST_HANDLER_HH
#define HEALTH_REQUEST_HANDLER_HH

#include "../http_request_handler.hh"

class HealthRequestHandler : public HttpRequestHandler {
public:
    REQUEST_HANDLER_NAME = "health-reqhandler";
};

#endif //HEALTH_REQUEST_HANDLER_HH
