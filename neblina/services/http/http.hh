#ifndef HTTP_HH
#define HTTP_HH

#include "service/communication/communication_service.hh"
#include "http_config.hh"
#include "http_connection.hh"
#include "arguments.hh"
#include "handler/http_routes.hh"

class Http final : public CommunicationService {
public:
    SERVICE_NAME = "http";

    Http(): config_(HttpConfig::from_file(config_filename())) {}

    void init() override;

    std::unique_ptr<TCPConnection> new_connection(int fd) const override {
        return std::make_unique<HttpConnection>(fd, routes_);
    }

private:
    HttpConfig config_;
    std::vector<HttpRoute> routes_;

    static fs::path config_filename() { return args().config_dir() / "http.json"; };
};

#endif //HTTP_HH
