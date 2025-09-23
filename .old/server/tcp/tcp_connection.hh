#ifndef TCP_CONNECTION_HH
#define TCP_CONNECTION_HH

#include <cstdint>

#include <string_view>
#include <vector>

#include "../connection.hh"

class TCPConnection : public Connection {
public:
    explicit TCPConnection(int fd, std::string const& host, std::string const& port) : fd_(fd), host_(host), port_(port) {}

    void close_connection();
    virtual ConnectionStatus handle_new_data();

    void send_data(uint8_t const* data, size_t sz) override;

    [[nodiscard]] ConnectionStatus connection_status() const { return connection_status_; }
    [[nodiscard]] std::string      host() const { return host_; }
    [[nodiscard]] std::string      port() const { return port_; }

protected:
    int              fd_;
    ConnectionStatus connection_status_ = ConnectionStatus::Open;
    std::string      host_;
    std::string      port_;

    static constexpr int BUFFER_SZ = 8 * 1024;
};


#endif //TCP_CONNECTION_HH
