#include "http_11.hh"

#include <iostream>

void Http11::new_data_available(std::string const& text, int fd)
{
    if (text.empty()) {
        send_data("HTTP/1.1 200 OK\r\n", fd);
        send_data("Content-Type: text/html; charset=UTF-8\r\n", fd);
        send_data("Content-Length: 14\r\n\r\n", fd);
        send_data("<h1>Hello</h1>", fd);
    }
}
