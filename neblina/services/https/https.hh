#ifndef HTTPS_HH
#define HTTPS_HH

#include "arguments.hh"
#include "https_config.hh"
#include "services/http/http_session.hh"
#include "service/communication/communication_service.hh"

class Https : public CommunicationService {
public:
    SERVICE_NAME = "https";

    Https();

    void new_connection(Connection* connection) override { sessions_.emplace(connection, std::make_unique<HttpSession>(connection, routes_) ); }
    void connection_closed(Connection* connection) override { sessions_.erase(connection); }

private:
    HttpsConfig config_;
    std::vector<HttpRoute> routes_;
    std::unordered_map<Connection*, std::unique_ptr<HttpSession>> sessions_;

    static fs::path config_filename() { return args().config_dir() / "https.json"; };
};

#endif //HTTPS_HH
