#ifndef HTTP_11_HH
#define HTTP_11_HH

#include "../service/tcp/tcp_server_text.hh"

class Http11 : public TCPServerText{
public:
    explicit Http11(Arguments const& args)
        : TCPServerText(args, 8080, false) {}

    static const char* name() { return "http11"; }

protected:
    void new_data_available(std::string const& text, int fd) override;
};

#endif //HTTP_11_HH
