#ifndef HTTP_HH
#define HTTP_HH

#include "service/tcp/tcp_service.hh"
#include "http_router_config.hh"
#include "http_router_connection.hh"

class HttpRouter final : public TCPService {
public:
    SERVICE_NAME = "http_router";

    HttpRouter(): config_(load_config_file()) {}

    std::unique_ptr<TCPConnection> new_connection(int fd) const override {
        return std::make_unique<HttpRouterConnection>(fd);
    }

private:
    HttpRouterConfig config_;

    static HttpRouterConfig load_config_file();
    static std::string      config_filename() { return config_dir() + "/http_router.json"; };
};

#endif //HTTP_HH
