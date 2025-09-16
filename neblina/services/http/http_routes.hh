#ifndef HTTP_ROUTES_HH
#define HTTP_ROUTES_HH

#include <regex>

#include "http_request_handler.hh"

struct HttpRoute {
    std::regex regex;
    std::unique_ptr<HttpRequestHandler> handler;
};


#endif //HTTP_ROUTES_HH
