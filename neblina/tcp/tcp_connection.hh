#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <cstdint>

#include <string_view>
#include <vector>

enum class ConnectionStatus { Open, Closed };

class TCPConnection {
public:
    explicit TCPConnection(int fd) : fd_(fd) {}
    virtual ~TCPConnection() = default;

    void close_connection();

    virtual void new_data_available(std::vector<uint8_t> const& data) = 0;

    [[nodiscard]] ConnectionStatus connection_status() const { return connection_status_; }

protected:
    int fd_;
    ConnectionStatus connection_status_ = ConnectionStatus::Open;

    void send_data(std::vector<uint8_t> const& data) { send_data(data.data(), data.size()); }
    void send_data(std::string_view data)            { send_data((uint8_t const*) data.data(), data.size()); }
    void send_data(uint8_t const* data, size_t sz);
};


#endif //CONNECTION_HH
