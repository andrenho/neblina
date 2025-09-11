#ifndef PARROT_TEXT_HH
#define PARROT_TEXT_HH

#include "service/tcp/tcp_server_text.hh"

class ParrotText : public TCPServerText {
public:
    explicit ParrotText(Arguments const& args) : TCPServerText(args, 23457, false) {}

protected:
    void new_data_available(std::string const& text, int fd) override
    {
        send_data((text + "\r\n").c_str(), fd);
    }

public:
    static const char* name() { return "parrot_text"; }
};

#endif //PARROT_TEXT_HH
