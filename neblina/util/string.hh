#ifndef STRING_HH
#define STRING_HH

#include <string>
#include <vector>
#include <ranges>
using namespace std::string_literals;

constexpr size_t npos = std::string::npos;

std::string to_upper(std::string text);
std::string to_lower(std::string text);

std::vector<std::string> split(const std::string& s, const std::string& delimiter);
inline std::vector<std::string> split(const std::string& s, char delimiter) { return split(s, std::string(1, delimiter)); }

#endif //STRING_HH
