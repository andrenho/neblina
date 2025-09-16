#ifndef STRING_HH
#define STRING_HH

#include <string>
#include <vector>
#include <ranges>
using namespace std::string_literals;

constexpr size_t npos = std::string::npos;

std::string to_upper(std::string text);
std::string to_lower(std::string text);

template<typename T>
std::vector<std::string> split(std::string_view text, T delimiter) {
    std::vector<std::string> tokens;
    auto split_view = text | std::views::split(delimiter);

    for (auto&& token : split_view) {
        if (!token.empty()) {  // Skip empty tokens
            tokens.emplace_back(token.begin(), token.end());
        }
    }

    return tokens;
}

#endif //STRING_HH
