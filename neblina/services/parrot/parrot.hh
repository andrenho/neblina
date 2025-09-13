#ifndef PARROT_TEXT_HH
#define PARROT_TEXT_HH

#include "service/tcp/tcp_server_text.hh"

class Parrot : public TCPServerText {
protected:
    void new_data_available(std::string const& text, int fd) override
    {
        send_data((text + "\r\n").c_str(), fd);
    }

public:
    static constexpr std::string_view name = "parrot";
};

#endif //PARROT_TEXT_HH
