#include "http_11.hh"

#include <iostream>

static const char* default_response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "\r\n"
    "<html><head><title>neblina</title></head><body>"
    "<h1>It works!</h1>"
    "<p>You have installed a HTTP <b>neblina</b> server. Make sure to configure it "
    "to make content available to your users.</p></body></html>\r\n";


void Http11::new_data_available(std::string const& text, int fd)
{
    if (text.empty()) {
        send_data(default_response, fd);
        close_connection(fd);
    }
}
