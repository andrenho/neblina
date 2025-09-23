#ifndef SERVER_HH
#define SERVER_HH

#include <functional>

#include "connection.hh"

using NewConnectionCallback = std::function<Connection*()>;

class IServerListener {
public:
    virtual ~IServerListener() = default;
    virtual void new_connection(Connection* connection) = 0;
    virtual void connection_closed(Connection* connection) = 0;
};

class Server {
public:
    virtual ~Server() = default;

    virtual void iteration() = 0;

    void set_listener(IServerListener* listener) { listener_ = listener; }

protected:
    IServerListener* listener_ = nullptr;
};

#endif //SERVER_HH
