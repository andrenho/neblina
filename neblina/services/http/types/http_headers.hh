#ifndef HTTP_HEADERS_HH
#define HTTP_HEADERS_HH

#include <optional>
#include <unordered_map>
#include <utility>
#include <string>

class HttpHeaders {
public:
    std::string&               operator[](std::string const& key);
    std::optional<std::string> at(std::string const& key) const;

    bool contains(std::string const& key) const;

    std::string to_string(size_t with_content_length=0) const;

private:
    std::unordered_map<std::string, std::string> headers_;
};

#endif //HTTP_HEADERS_HH
