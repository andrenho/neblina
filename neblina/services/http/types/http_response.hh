#ifndef HTTP_RESPONSE_HH
#define HTTP_RESPONSE_HH

#include <string>

#include "content_type.hh"
#include "http_headers.hh"

struct HttpResponse {
    HttpResponse() = default;
    explicit HttpResponse(int status_code);
    HttpResponse(int status_code, std::string_view content_type, std::string const& message);

    int         status_code;
    HttpHeaders headers;
    std::string body;

    void compress();

    std::string to_string() const;

    static HttpResponse error_response_html(int status_code, std::string const& message);

    [[nodiscard]] static std::string http_reason_phrase(int code);
};

#endif //HTTP_RESPONSE_HH
