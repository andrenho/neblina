#ifndef TCP_SERVICE_HH
#define TCP_SERVICE_HH

#include "service/service.hh"
#include "../../server/tcp_server.hh"

template <typename Conn>
class TCPService : public Service {
public:
    TCPService() : tcp_server_([](int fd) { return std::make_unique<Conn>(fd); }) {}

    void run() override { tcp_server_.run(); }

private:
    TCPServer tcp_server_;
};

#endif //TCP_SERVICE_HH
