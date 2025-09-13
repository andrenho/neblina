#include "http.hh"

void Http::new_data_available(std::string const& text, int fd)
{
    it_works(fd);
}

void Http::it_works(int fd)
{
    static const char* it_works_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "\r\n"
        "<html><head><title>neblina</title></head><body>"
        "<h1>It works!</h1>"
        "<p>You have installed a HTTP <b>neblina</b> server. Make sure to configure it "
        "to make content available to your users.</p></body></html>\r\n";

    send_data(it_works_response, fd);
    close_connection(fd);
}
