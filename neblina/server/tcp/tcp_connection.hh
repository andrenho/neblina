#ifndef TCP_CONNECTION_HH
#define TCP_CONNECTION_HH

#include <cstdint>

#include <string_view>
#include <vector>

#include "../connection.hh"

class TCPConnection : public Connection {
public:
    explicit TCPConnection(int fd) : fd_(fd) {}

    void close_connection();
    virtual ConnectionStatus handle_new_data();

    [[nodiscard]] ConnectionStatus connection_status() const { return connection_status_; }

protected:
    int fd_;
    ConnectionStatus connection_status_ = ConnectionStatus::Open;

    void send_data(std::vector<uint8_t> const& data) { send_data(data.data(), data.size()); }
    void send_data(std::string_view data)            { send_data((uint8_t const*) data.data(), data.size()); }
    void send_data(uint8_t const* data, size_t sz);

private:
    static constexpr int BUFFER_SZ = 8 * 1024;
};


#endif //TCP_CONNECTION_HH
