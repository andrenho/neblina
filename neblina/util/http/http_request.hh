#ifndef HTTP_REQUEST_HH
#define HTTP_REQUEST_HH

#include <unordered_map>
#include <stdexcept>
#include <string>

using HttpHeaders = std::unordered_map<std::string, std::string>;

class HttpRequest {
public:
    void operator<<(std::string_view data);

    enum class Method { Get, Post, Put, Delete, Patch, Options, Head, Trace, Connect, Undefined };

    [[nodiscard]] Method      method()   const { return method_; }
    [[nodiscard]] std::string resource() const { return resource_; }
    [[nodiscard]] HttpHeaders headers()  const { return headers_; }
    [[nodiscard]] std::string body()     const { return body_; }
    [[nodiscard]] bool        complete() const { return request_stage_ == RequestStage::Done; }

    class BadRequest             final : public std::exception {};  // 400
    class DataAfterBody          final : public std::exception {};  // 413 (?)
    class UnsupportedHttpVersion final : public std::exception {};  // 505
    class NotSupportedYet        final : public std::runtime_error { using std::runtime_error::runtime_error; };

private:
    enum class RequestStage { StartLine, Headers, Body, Done };
    RequestStage request_stage_ = RequestStage::StartLine;

    Method      method_ = Method::Undefined;
    std::string resource_ = "/";
    HttpHeaders headers_;
    std::string body_;
    size_t      content_length_;

    void process_start_line(std::string_view data);
    void process_header(std::string_view data);
    void headers_end();
    void process_body(std::string_view data);

    [[nodiscard]] Method translate_method(std::string const& method) const;
};

#endif //HTTP_REQUEST_HH
