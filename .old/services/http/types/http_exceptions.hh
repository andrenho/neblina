#ifndef HTTP_EXCEPTIONS_HH
#define HTTP_EXCEPTIONS_HH

#include <stdexcept>

#include "http_response.hh"

struct HttpException : std::runtime_error {
    explicit HttpException(int status_code) : std::runtime_error(HttpResponse::http_reason_phrase((status_code))), status_code(status_code) {}
    explicit HttpException(int status_code, const char* description) : std::runtime_error(description), status_code(status_code) {}
    const int status_code;
};

#define EXCEPTION(name, code) struct name ## Exception final : public HttpException { name ## Exception() : HttpException(code) {} explicit name ## Exception(const char* description) : HttpException(code, description) {} };

EXCEPTION(BadRequest, 400)
EXCEPTION(Unauthorized, 401)
EXCEPTION(Forbidden, 403)
EXCEPTION(NotFound, 404)
EXCEPTION(MethodNotAllowed, 405)
EXCEPTION(NotAcceptable, 406)
EXCEPTION(RequestTimeout, 408)
EXCEPTION(ContentTooLarge, 413)
EXCEPTION(UnsupportedMediaType, 415)
EXCEPTION(TooManyRequests, 429)
EXCEPTION(InternalServerError, 500)
EXCEPTION(NotImplemented, 501)
EXCEPTION(BadGateway, 502)
EXCEPTION(ServiceUnavailable, 503)
EXCEPTION(GatewayTimeout, 504)
EXCEPTION(HttpVersionNotSupported, 505)

#undef EXCEPTION

#endif //HTTP_EXCEPTIONS_HH
