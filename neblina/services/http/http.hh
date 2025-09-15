#ifndef HTTP_HH
#define HTTP_HH

#include "service/tcp/tcp_service.hh"
#include "http_config.hh"
#include "http_connection.hh"
#include "arguments.hh"

class Http final : public TCPService {
public:
    SERVICE_NAME = "http";

    Http(): config_(load_config_file()) {}

    void init() override;

    std::unique_ptr<TCPConnection> new_connection(int fd) const override {
        return std::make_unique<HttpConnection>(fd, config_);
    }

private:
    HttpConfig config_;

    static HttpConfig   load_config_file();
    static std::string  config_filename() { return args().config_dir() + "/http.json"; };
};

#endif //HTTP_HH
