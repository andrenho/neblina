#include "http_headers.hh"

#include <algorithm>
#include <cctype>
#include <format>
#include <ranges>

#include "util/chrono.hh"

static std::string to_upper(std::string const& key)
{
    std::string up = key;
    std::ranges::transform(up, up.begin(), ::toupper);
    return up;
}

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

std::string const& HttpHeaders::at(std::string const& key) const
{
    return headers_.at(to_upper(key));
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
