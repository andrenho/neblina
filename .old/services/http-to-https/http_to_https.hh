#ifndef HTTP_TO_HTTPS_HH
#define HTTP_TO_HTTPS_HH

#include <memory>

#include "service/communication/communication_service.hh"
#include "services/http/handlers/redirect_request_handler.hh"
#include "server/tcp/tcp_server.hh"

class HttpToHttps : public CommunicationService {
public:
    SERVICE_NAME = "http-to-https";

    HttpToHttps() : CommunicationService(std::make_unique<TCPServer>()) {
        routes_.emplace_back(std::regex("(.*)"), std::make_unique<RedirectRequestHandler>("https://localhost"));
    }

    void new_connection(Connection* connection) override { sessions_.emplace(connection, std::make_unique<HttpSession>(connection, routes_) ); }
    void connection_closed(Connection* connection) override { sessions_.erase(connection); }

private:
    std::vector<HttpRoute> routes_;
    std::unordered_map<Connection*, std::unique_ptr<HttpSession>> sessions_;
};



#endif //HTTP_TO_HTTPS_HH
