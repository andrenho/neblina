#ifndef TCP_SERVICE_HH
#define TCP_SERVICE_HH

#include <unordered_map>

#include "server/server.hh"
#include "service/service.hh"

class CommunicationService : public Service, IServerListener {
public:
    explicit CommunicationService(std::unique_ptr<Server> server) : server_(std::move(server)) {
        server_->set_listener(this);
    }

    void run() override {
        while (!should_exit())
            server_->iteration();
    }

private:
    std::unique_ptr<Server> server_;
};

#endif //TCP_SERVICE_HH
