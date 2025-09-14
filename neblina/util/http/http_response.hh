#ifndef HTTP_RESPONSE_HH
#define HTTP_RESPONSE_HH

#include <string>

#include "http_headers.hh"

struct HttpResponse {
    HttpResponse() = default;
    explicit HttpResponse(int status_code);
    HttpResponse(int status_code, std::string message);

    int         status_code;
    HttpHeaders headers;
    std::string body;

    std::string to_string() const;

    [[nodiscard]] static std::string http_reason_phrase(int code);
};

#endif //HTTP_RESPONSE_HH
