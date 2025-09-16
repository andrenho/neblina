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