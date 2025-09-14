#ifndef HTTP_REQUEST_HANDLER_HH
#define HTTP_REQUEST_HANDLER_HH

#include <unordered_map>
#include <string>
#include <vector>

#include "http_exceptions.hh"
#include "util/http/http_request.hh"
#include "util/http/http_response.hh"

using QueryParameters = std::unordered_map<std::string, std::string>;
using URLParameters = std::vector<std::string>;

class HttpRequestHandler {
public:
    virtual ~HttpRequestHandler() = default;

    virtual HttpResponse get(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters) const { throw MethodNotAllowedException(); }
    virtual HttpResponse post(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters) const { throw MethodNotAllowedException(); }
    virtual HttpResponse patch(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters) const { throw MethodNotAllowedException(); }
    virtual HttpResponse put(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters) const { throw MethodNotAllowedException(); }
    virtual HttpResponse delete_(HttpRequest const& request, URLParameters const& url_parameters, QueryParameters const& query_parameters) const { throw MethodNotAllowedException(); }

    virtual HttpResponse head(HttpRequest const& request,URLParameters const& url_parameters, QueryParameters const& query_parameters) const {
        HttpResponse r = get(request, url_parameters, query_parameters);
        r.body = "";
        return r;
    }
};

#endif //HTTP_REQUEST_HANDLER_HH
