#ifndef PARROT_TEXT_HH
#define PARROT_TEXT_HH

#include "service/tcp/tcp_connection.hh"
#include "service/tcp/tcp_service.hh"

class ParrotConnection final : public TCPConnection {
public:
    using TCPConnection::TCPConnection;

    void new_data_available(std::vector<uint8_t> const& data) override
    {
        send_data(data);
    }

    /*
    void new_data_available(std::string const& text) override
    {
        send_data(text + "\r\n");
    }
    */

};

class Parrot final : public TCPService {
public:
    std::unique_ptr<TCPConnection> new_connection(int fd) const override { return std::make_unique<ParrotConnection>(fd); }

    static constexpr std::string_view name = "parrot";
};

#endif //PARROT_TEXT_HH
