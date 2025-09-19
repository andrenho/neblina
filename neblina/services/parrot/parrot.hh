#ifndef PARROT_TEXT_HH
#define PARROT_TEXT_HH

#include "server/tcp/tcp_server.hh"
#include "service/communication/communication_service.hh"
#include "service/communication/session.hh"

class ParrotSession final : public Session {
public:
    explicit ParrotSession(Connection* connection)
        : Session(connection, "\r\n") {}

    ConnectionStatus new_line(std::string_view data) override {
        send_data(data);
        send_data("\r\n");
        return ConnectionStatus::Open;
    }
};

class Parrot final : public CommunicationService {
public:
    SERVICE_NAME = "parrot";

    Parrot() : CommunicationService(std::make_unique<TCPServer>()) {}

    void new_connection(Connection* connection) override { sessions_.emplace(connection, std::make_unique<ParrotSession>(connection) ); }
    void connection_closed(Connection* connection) override { sessions_.erase(connection); }

private:
    std::unordered_map<Connection*, std::unique_ptr<ParrotSession>> sessions_;
};

#endif //PARROT_TEXT_HH
