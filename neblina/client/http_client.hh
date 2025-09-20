#ifndef HTTP_CLIENT_HH
#define HTTP_CLIENT_HH

#include <string>

#include "services/http/types/http_request.hh"
#include "services/http/types/http_response.hh"

HttpResponse http_client_request(HttpRequest const& request, int port);

#endif //HTTP_CLIENT_HH
