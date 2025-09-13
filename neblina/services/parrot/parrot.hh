#ifndef PARROT_TEXT_HH
#define PARROT_TEXT_HH

#include "service/tcp/tcp_connection_text.hh"
#include "service/tcp/tcp_service.hh"

class ParrotConnection final : public TCPConnectionText {
public:
    using TCPConnectionText::TCPConnectionText;

    void new_data_available(std::string_view text) override
    {
        send_data(text);
        send_data("\r\n");
    }

};

class Parrot final : public TCPService {
public:
    std::unique_ptr<TCPConnection> new_connection(int fd) const override { return std::make_unique<ParrotConnection>(fd); }

    static constexpr std::string_view name = "parrot";
};

#endif //PARROT_TEXT_HH
