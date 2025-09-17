#ifndef STATIC_REQUEST_HANDLER_HH
#define STATIC_REQUEST_HANDLER_HH

#include <optional>
#include "../http_request_handler.hh"

class StaticRequestHandler : public HttpRequestHandler {
public:
    [[nodiscard]] HttpResponse get(const HttpRequest& request, const URLParameters& url_parameters,
        const QueryParameters& query_parameters) const override;

protected:
    [[nodiscard]] virtual HttpResponse file_contents(std::vector<std::string> const& file_path, std::string const& cached_etag) const = 0;
};

#endif //STATIC_REQUEST_HANDLER_HH
