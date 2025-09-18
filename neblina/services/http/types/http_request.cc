#include "http_request.hh"

#include <cctype>

#include <algorithm>
#include <sstream>

#include "http_exceptions.hh"

void HttpRequest::operator<<(std::string_view data)
{
    switch (request_stage_) {
        case RequestStage::StartLine:
            process_start_line(data);
            break;
        case RequestStage::Headers:
            if (data.empty())
                headers_end();
            else
                process_header(data);
            break;
        case RequestStage::Body:
            process_body(data);
            break;
        case RequestStage::Done:
            throw ContentTooLargeException();
    }
}

void HttpRequest::process_start_line(std::string_view data)
{
    std::istringstream iss(data.data());
    std::string        method, version, extra;

    if (!(iss >> method >> resource_ >> version))
        throw BadRequestException();
    if (iss >> extra)
        throw BadRequestException();

    method_ = translate_method(method);
    if (method_ == Method::Undefined)
        throw BadRequestException();

    if (version != "HTTP/1.1")
        throw HttpVersionNotSupportedException();

    request_stage_ = RequestStage::Headers;
}

void HttpRequest::process_header(std::string_view data)
{
    auto const sep_idx = data.find(':');
    if (sep_idx == std::string::npos)
        throw BadRequestException();

    std::string key = std::string(data.substr(0, sep_idx));

    auto const f = data.find_first_not_of(' ', sep_idx + 1);
    if (f == std::string::npos)
        throw BadRequestException();
    std::string value = std::string(data.substr(f, data.size() - 2));

    if (headers_.contains(key))
        headers_[key] = *headers_.at(key) + ", " + value;
    else
        headers_[key] = value;
}

void HttpRequest::headers_end()
{
    if (headers_.contains("Content-Length")) {
        request_stage_ = RequestStage::Body;
        content_length_ = strtoll(headers_.at("Content-Length")->c_str(), nullptr, 10);
    } else {
        request_stage_ = RequestStage::Done;
    }

    if (headers_.contains("Transfer-Encoding"))
        throw NotImplementedException("Transfer-Encoding header");
}

void HttpRequest::process_body(std::string_view data)
{
    body_ += data;
    body_ += "\r\n";
    if (body_.size() >= content_length_)
        request_stage_ = RequestStage::Done;
}

HttpRequest::Method HttpRequest::translate_method(std::string const& method)
{
    if (method == "GET")     return Method::Get;
    if (method == "HEAD")    return Method::Head;
    if (method == "POST")    return Method::Post;
    if (method == "PUT")     return Method::Put;
    if (method == "DELETE")  return Method::Delete;
    if (method == "CONNECT") return Method::Connect;
    if (method == "OPTIONS") return Method::Options;
    if (method == "TRACE")   return Method::Trace;
    if (method == "PATCH")   return Method::Patch;
    return Method::Undefined;
}
