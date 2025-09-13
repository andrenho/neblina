#ifndef PARROT_TEXT_HH
#define PARROT_TEXT_HH

#include "service/tcp/tcp_connection.hh"
#include "service/tcp/tcp_service.hh"

class ParrotConnection final : public TCPConnection {
protected:
    using TCPConnection::TCPConnection;

public:
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

class Parrot final : public TCPService<ParrotConnection> {
public:
    static constexpr std::string_view name = "parrot";
};

#endif //PARROT_TEXT_HH
