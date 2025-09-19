#ifndef TCP_SERVICE_HH
#define TCP_SERVICE_HH

#include <unordered_map>

#include "server/server.hh"
#include "service/service.hh"

class Server;

template <typename SessionType>
class CommunicationService : public Service, IServerListener {
public:
    explicit CommunicationService(std::unique_ptr<Server> server) : server_(std::move(server)) {
        server_->set_listener(this);
    }
    void run() override {
        while (!should_exit())
            server_->iteration();
    }

    void new_connection(Connection* connection) override {
        sessions_.emplace(connection, connection);
    }

    void connection_closed(Connection* connection) override {
        sessions_.erase(connection);
    }

private:
    std::unique_ptr<Server> server_;
    std::unordered_map<Connection*, SessionType> sessions_;
};

#endif //TCP_SERVICE_HH
