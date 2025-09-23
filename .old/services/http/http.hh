#ifndef HTTP_HH
#define HTTP_HH

#include "service/communication/communication_service.hh"
#include "http_config.hh"
#include "http_session.hh"
#include "arguments.hh"
#include "handler/http_routes.hh"
#include "server/tcp/tcp_server.hh"

class Http final : public CommunicationService {
public:
    SERVICE_NAME = "http";

    Http();

    void init() override;

    void new_connection(Connection* connection) override { sessions_.emplace(connection, std::make_unique<HttpSession>(connection, routes_) ); }
    void connection_closed(Connection* connection) override { sessions_.erase(connection); }

private:
    HttpConfig config_;
    std::vector<HttpRoute> routes_;
    std::unordered_map<Connection*, std::unique_ptr<HttpSession>> sessions_;

    static fs::path config_filename() { return args().config_dir() / "http.json"; };
};

#endif //HTTP_HH
