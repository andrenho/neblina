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

    static constexpr std::string_view best_content_type(std::string_view extension)
    {
        if (extension == ".htm" || extension == ".html")
            return HTML;
        if (extension == ".txt" || extension == ".text")
            return PLAIN;
        if (extension == ".css")
            return CSS;
        if (extension == ".js")
            return JS;
        if (extension == ".json")
            return JSON;
        if (extension == ".xml")
            return XML;
        if (extension == ".png")
            return PNG;
        if (extension == ".jpg" || extension == ".jpeg")
            return JPEG;
        if (extension == ".gif")
            return GIF;
        if (extension == ".webp")
            return WEBP;
        if (extension == ".svg")
            return SVG;
        if (extension == ".pdf")
            return PDF;
        if (extension == ".zip")
            return ZIP;
        return OCTET;
    }
};

#endif //CONTENT_TYPE_HH
