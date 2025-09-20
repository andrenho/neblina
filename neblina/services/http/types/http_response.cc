#include "http_response.hh"

#include <format>
#include <string>
#include <vector>

#include "file/gz.hh"
#include "util/string.hh"

HttpResponse::HttpResponse(int status_code)
    : status_code(status_code)
{
    headers["Server"] = "neblina";
}

HttpResponse::HttpResponse(int status_code, std::string_view content_type, std::string const& message)
    : HttpResponse(status_code)
{
    headers["Content-Type"] = content_type;
    body = message;
}

HttpResponse HttpResponse::from_string(std::string const& str)
{
    HttpResponse r;

    auto lines = split(str, "\r\n");

    auto line0 = split(lines.at(0), " ");
    if (line0.size() != 3)
        throw std::runtime_error("Malformed request");
    //if (line0.at(0) != "HTTP/1.1")
    //    throw std::runtime_error("Unsupported HTTP version");
    r.status_code = std::stoi(line0.at(1));

    for (size_t ln = 1; !lines.at(ln).empty(); ++ln) {
        size_t i = lines.at(ln).find(':');
        if (i == std::string::npos)
            throw std::runtime_error("Malformed request");
        std::string key = lines.at(ln).substr(0, i);
        while (key.at(key.size() - 1) == ' ')
            key = key.substr(0, key.size() - 1);
        std::string value = lines.at(ln).substr(i + 1);
        while (value.at(0) == ' ')
            value = value.substr(1);
        r.headers[key] = value;
    }

    size_t i = str.find("\r\n\r\n");
    if (i != std::string::npos)
        r.body = str.substr(i + 4);

    return r;
}

HttpResponse HttpResponse::error_response_html(int status_code, std::string const& message)
{
    std::string reason = http_reason_phrase(status_code);
    return HttpResponse(status_code, ContentType::HTML,
    std::format("<html><head><title>{} {}</title></head><body><h1>{} {}</h1><p>{}</p></body></html>",
            status_code, reason, status_code, reason, message));
}

HttpResponse HttpResponse::redirect_response(std::string const& new_location)
{
    HttpResponse response(301, ContentType::TXT, "Moved Permanently. Redirecting to " + new_location);
    response.headers["Cache-Control"] = "max-age=2592000,public";
    response.headers["Location"] = new_location;
    return response;
}

std::string HttpResponse::http_reason_phrase(int code)
{
    switch (code) {
        // 1xx Informational
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 102: return "Processing"; // WebDAV
        case 103: return "Early Hints";

        // 2xx Success
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 207: return "Multi-Status"; // WebDAV
        case 208: return "Already Reported"; // WebDAV
        case 226: return "IM Used";

        // 3xx Redirection
        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy"; // deprecated
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";

        // 4xx Client Error
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Payload Too Large";
        case 414: return "URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Range Not Satisfiable";
        case 417: return "Expectation Failed";
        case 418: return "I'm a teapot"; // RFC 2324
        case 421: return "Misdirected Request";
        case 422: return "Unprocessable Content"; // WebDAV
        case 423: return "Locked"; // WebDAV
        case 424: return "Failed Dependency"; // WebDAV
        case 425: return "Too Early";
        case 426: return "Upgrade Required";
        case 428: return "Precondition Required";
        case 429: return "Too Many Requests";
        case 431: return "Request Header Fields Too Large";
        case 451: return "Unavailable For Legal Reasons";

        // 5xx Server Error
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";
        case 506: return "Variant Also Negotiates";
        case 507: return "Insufficient Storage"; // WebDAV
        case 508: return "Loop Detected"; // WebDAV
        case 510: return "Not Extended";
        case 511: return "Network Authentication Required";

        default: return "Unknown status code";
    }
}

std::string HttpResponse::to_string() const
{
    return "HTTP/1.1 " + std::to_string(status_code) + " " + http_reason_phrase(status_code) + "\r\n"
        + headers.to_string(body.size()) + "\r\n"
        + body;
}

void HttpResponse::compress()
{
    static std::vector<std::string_view> compressable = {
        ContentType::HTML,
        ContentType::TXT,
        ContentType::JS,
        ContentType::JSON,
        ContentType::CSS,
        ContentType::BIN,
        ContentType::BMP
    };

    if (std::find(compressable.begin(), compressable.end(), headers.content_type()) != compressable.end()) {
        auto zip = gz::gzip({ body.begin(), body.end() });
        body = { zip.begin(), zip.end() };
        headers["Content-Encoding"] = "gzip";
    }
}
