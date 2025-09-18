#ifndef HTTP_REQUEST_HH
#define HTTP_REQUEST_HH

#include <string>

#include "http_headers.hh"

class HttpRequest {
public:
    void operator<<(std::string_view data);

    enum class Method { Get, Post, Put, Delete, Patch, Options, Head, Trace, Connect, Undefined };

    [[nodiscard]] Method      method()   const { return method_; }
    [[nodiscard]] std::string resource() const { return resource_; }
    [[nodiscard]] HttpHeaders headers()  const { return headers_; }
    [[nodiscard]] std::string body()     const { return body_; }
    [[nodiscard]] bool        complete() const { return request_stage_ == RequestStage::Done; }

private:
    enum class RequestStage { StartLine, Headers, Body, Done };
    RequestStage request_stage_ = RequestStage::StartLine;

    Method      method_ = Method::Undefined;
    std::string resource_ = "/";
    HttpHeaders headers_;
    std::string body_;
    size_t      content_length_ = 0;

    void process_start_line(std::string_view data);
    void process_header(std::string_view data);
    void headers_end();
    void process_body(std::string_view data);

    [[nodiscard]] static Method translate_method(std::string const& method);
};

#endif //HTTP_REQUEST_HH
