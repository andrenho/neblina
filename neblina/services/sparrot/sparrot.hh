#ifndef SPARROT_TEXT_HH
#define SPARROT_TEXT_HH

#include "server/ssl/ssl_server.hh"
#include "server/tcp/tcp_server.hh"
#include "service/communication/communication_service.hh"
#include "service/communication/session.hh"

class SParrotSession final : public Session {
public:
    explicit SParrotSession(Connection* connection)
        : Session(connection, "\n") {}

    ConnectionStatus new_line(std::string_view data) override {
        send_data(data);
        send_data("\n");
        return ConnectionStatus::Open;
    }
};

class SParrot final : public CommunicationService {
public:
    SERVICE_NAME = "sparrot";

    SParrot() : CommunicationService(std::make_unique<SSLServer>(args().data_dir / "https/cert.pem", args().data_dir / "https/key.pem")) {}

    void new_connection(Connection* connection) override { sessions_.emplace(connection, std::make_unique<SParrotSession>(connection) ); }
    void connection_closed(Connection* connection) override { sessions_.erase(connection); }

private:
    std::unordered_map<Connection*, std::unique_ptr<SParrotSession>> sessions_;
};

#endif //SPARROT_TEXT_HH
