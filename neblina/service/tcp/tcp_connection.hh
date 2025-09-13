#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <cstdint>
#include <cstring>
#include <sys/socket.h>

#include <string_view>
#include <vector>

#include "service/service.hh"
#include "util/log.hh"

enum class ConnectionStatus { Open, Closed };

class TCPConnection {
public:
    explicit TCPConnection(int fd) : fd_(fd) {}
    virtual ~TCPConnection() = default;

    virtual void new_data_available(std::vector<uint8_t> const& data) = 0;

    [[nodiscard]] ConnectionStatus connection_status() const { return connection_status_; }

protected:
    void send_data(std::vector<uint8_t> const& data) { send_data(data.data(), data.size()); }
    void send_data(std::string_view data) { send_data(reinterpret_cast<uint8_t const*>(data.data()), data.size()); }
    void send_data(uint8_t const* data, size_t sz);

private:
    int fd_;
    ConnectionStatus connection_status_ = ConnectionStatus::Open;
};


#endif //CONNECTION_HH
