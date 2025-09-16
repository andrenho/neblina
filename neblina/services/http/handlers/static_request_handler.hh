#ifndef STATIC_REQUEST_HANDLER_HH
#define STATIC_REQUEST_HANDLER_HH

#include <optional>
#include "../http_request_handler.hh"

class StaticRequestHandler : public HttpRequestHandler {
public:
    [[nodiscard]] HttpResponse get(const HttpRequest& request, const URLParameters& url_parameters,
        const QueryParameters& query_parameters) const override;

protected:
    [[nodiscard]] virtual std::optional<std::string> file_contents(std::vector<std::string> const& file_path) const = 0;
};

#endif //STATIC_REQUEST_HANDLER_HH
