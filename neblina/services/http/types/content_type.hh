#ifndef CONTENT_TYPE_HH
#define CONTENT_TYPE_HH

#include <string_view>

struct ContentType {
    static constexpr std::string_view HTML  = "text/html";
    static constexpr std::string_view PLAIN = "text/plain";
    static constexpr std::string_view CSS   = "text/css";
    static constexpr std::string_view JS    = "application/javascript";

    static constexpr std::string_view JSON  = "application/json";
    static constexpr std::string_view XML   = "application/xml";

    static constexpr std::string_view PNG   = "image/png";
    static constexpr std::string_view JPEG  = "image/jpeg";
    static constexpr std::string_view GIF   = "image/gif";
    static constexpr std::string_view WEBP  = "image/webp";
    static constexpr std::string_view SVG   = "image/svg+xml";

    static constexpr std::string_view PDF   = "application/pdf";
    static constexpr std::string_view ZIP   = "application/zip";
    static constexpr std::string_view OCTET = "application/octet-stream";
};

#endif //CONTENT_TYPE_HH
