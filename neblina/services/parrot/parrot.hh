#ifndef PARROT_TEXT_HH
#define PARROT_TEXT_HH

#include "server/tcp/tcp_server.hh"
#include "service/communication/communication_service.hh"
#include "service/communication/session.hh"

class ParrotSession final : public Session {
public:
    using Session::Session;

    ConnectionStatus new_data_available(std::vector<uint8_t> const& data) override {
        DBG("New data");
        return ConnectionStatus::Closed;
    }
};

class Parrot final : public CommunicationService<ParrotSession> {
public:
    explicit Parrot() : CommunicationService(std::make_unique<TCPServer>()) {}

    SERVICE_NAME = "parrot";
};

#endif //PARROT_TEXT_HH
