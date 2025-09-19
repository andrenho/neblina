#ifndef SESSION_HH
#define SESSION_HH

#include "server/connection.hh"

class Session : public IConnectionListener {
public:
    explicit Session(Connection* connection, std::string const& separator) : IConnectionListener(separator), connection_(connection) {
        connection_->set_listener(this);
    }

    explicit Session(Connection* connection) : IConnectionListener(), connection_(connection) {
        connection_->set_listener(this);
    }

    template <typename T>
    void send_data(T&& args...) { connection_->send_data(std::forward<T>(args)); }

private:
    Connection* connection_;
};

#endif //SESSION_HH
