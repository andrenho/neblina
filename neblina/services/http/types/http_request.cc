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
    std::string        smethod, version, extra;

    if (!(iss >> smethod >> resource >> version))
        throw BadRequestException();
    if (iss >> extra)
        throw BadRequestException();

    method = translate_method(smethod);
    if (method == Method::Undefined)
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

    if (headers.contains(key))
        headers[key] = *headers.at(key) + ", " + value;
    else
        headers[key] = value;
}

void HttpRequest::headers_end()
{
    if (headers.contains("Content-Length")) {
        request_stage_ = RequestStage::Body;
        content_length_ = strtoll(headers.at("Content-Length")->c_str(), nullptr, 10);
    } else {
        request_stage_ = RequestStage::Done;
    }

    if (headers.contains("Transfer-Encoding"))
        throw NotImplementedException("Transfer-Encoding header");
}

void HttpRequest::process_body(std::string_view data)
{
    body += data;
    body += "\r\n";
    if (body.size() >= content_length_)
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
    throw MethodNotAllowedException();
}

std::string HttpRequest::translate_method(Method method)
{
    if (method == Method::Get)     return "GET";
    if (method == Method::Head)    return "HEAD";
    if (method == Method::Post)    return "POST";
    if (method == Method::Put)     return "PUT";
    if (method == Method::Delete)  return "DELETE";
    if (method == Method::Connect) return "CONNECT";
    if (method == Method::Options) return "OPTIONS";
    if (method == Method::Trace)   return "TRACE";
    if (method == Method::Patch)   return "PATCH";
    throw MethodNotAllowedException();
}

std::string HttpRequest::to_string() const
{
    return translate_method(method) + " " + resource + " HTTP/1.1\r\n"
        + headers.to_string(body.size()) + "\r\n"
        + body;
}
