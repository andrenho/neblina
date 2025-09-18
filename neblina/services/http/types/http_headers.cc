#include "http_headers.hh"

#include <cctype>
#include <format>
#include <ranges>

#include "util/chrono.hh"
#include "util/string.hh"

static std::string to_header_case(std::string_view input) {
    std::string result;
    result.reserve(input.size());

    bool capitalize = true;
    for (char ch : input) {
        if (ch == '-') {
            result.push_back('-');
            capitalize = true;
        } else {
            if (capitalize)
                result.push_back(std::toupper(static_cast<unsigned char>(ch)));
            else
                result.push_back(std::tolower(static_cast<unsigned char>(ch)));
            capitalize = false;
        }
    }

    return result;
}

std::string& HttpHeaders::operator[](std::string const& key)
{
    return headers_[to_upper(key)];
}

std::optional<std::string> HttpHeaders::at(std::string const& key) const
{
    auto it = headers_.find(to_upper(key));
    if (it != headers_.end())
        return std::string(it->second);
    else
        return {};
}

bool HttpHeaders::contains(std::string const& key) const
{
    return headers_.contains(to_upper(key));
}

std::string HttpHeaders::to_string(size_t with_content_length) const
{
    // headers
    std::string str;
    for (auto const& kv: headers_) {
        if (with_content_length != 0 && (kv.first == "CONTENT-LENGTH" || kv.first == "DATE"))
            continue;
        str += to_header_case(kv.first) + ": " + kv.second + "\r\n";
    }

    // Content-Length
    if (with_content_length != 0)
        str += "Content-Length: " + std::to_string(with_content_length) + "\r\n";

    // Date
    auto now_in_seconds = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    str += std::format("Date: {:%a, %d %b %Y %H:%M:%S} GMT\r\n", now_in_seconds);
    return str;
}

bool HttpHeaders::accepts_encoding(std::string const& encoding) const
{
    return at("Accept-Encoding") && at("Accept-Encoding")->find(encoding) != npos;
}
