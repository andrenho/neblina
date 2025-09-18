#ifndef TCP_SERVICE_HH
#define TCP_SERVICE_HH

#include "service/service.hh"
#include "tcp/tcp_server.hh"
#include "../../tcp/tcp_connection.hh"

class TCPService : public Service {
public:
    TCPService() : tcp_server_(this) {}

    virtual std::unique_ptr<TCPConnection> new_connection(int fd) const = 0;
    void run() override { tcp_server_.run([this]() { return should_exit(); }); }

private:
    TCPServer tcp_server_;
};

#endif //TCP_SERVICE_HH
