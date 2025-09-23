#ifndef HTTP_REQUEST_HH
#define HTTP_REQUEST_HH

#include <string>

#include "http_headers.hh"

class HttpRequest {
public:
    void operator<<(std::string_view data);

    enum class Method { Get, Post, Put, Delete, Patch, Options, Head, Trace, Connect, Undefined };

    Method      method = Method::Undefined;
    std::string resource = "/";
    HttpHeaders headers;
    std::string body;

    [[nodiscard]] bool complete() const { return request_stage_ == RequestStage::Done; }

    std::string to_string() const;

private:
    enum class RequestStage { StartLine, Headers, Body, Done };
    RequestStage request_stage_ = RequestStage::StartLine;

    size_t      content_length_ = 0;

    void process_start_line(std::string_view data);
    void process_header(std::string_view data);
    void headers_end();
    void process_body(std::string_view data);

    [[nodiscard]] static Method      translate_method(std::string const& method);
    [[nodiscard]] static std::string translate_method(Method method);
};

#endif //HTTP_REQUEST_HH
