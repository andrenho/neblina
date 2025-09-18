#ifndef HTTP_HEADERS_HH
#define HTTP_HEADERS_HH

#include <optional>
#include <unordered_map>
#include <string>

class HttpHeaders {
public:
    std::string& operator[](std::string const& key);
    [[nodiscard]] std::optional<std::string> at(std::string const& key) const;

    [[nodiscard]] std::string to_string(size_t with_content_length=0) const;

    [[nodiscard]] bool contains(std::string const& key) const;

    [[nodiscard]] std::string connection() const    { auto h = at("Connection"); if (h) return *h; else return ""; }
    [[nodiscard]] std::string content_type() const  { auto h = at("Content-Type"); if (h) return *h; else return ""; }
    [[nodiscard]] std::string if_none_match() const { auto h = at("If-None-Match"); if (h) return *h; else return ""; }
    [[nodiscard]] bool accepts_encoding(std::string const& encoding) const;

private:
    std::unordered_map<std::string, std::string> headers_;
};

#endif //HTTP_HEADERS_HH
