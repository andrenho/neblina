#include "string.hh"

#include <algorithm>
#include <cctype>

std::string to_upper(std::string text)
{
    std::ranges::transform(text, text.begin(), ::toupper);
    return text;
}

std::string to_lower(std::string text)
{
    std::ranges::transform(text, text.begin(), ::tolower);
    return text;
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}