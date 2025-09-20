#ifndef HTTP_CLIENT_HH
#define HTTP_CLIENT_HH

#include <string>

#include "services/http/types/http_request.hh"
#include "services/http/types/http_response.hh"

HttpResponse http_client_request(std::string const& location, int port, HttpRequest const& request);

#endif //HTTP_CLIENT_HH
