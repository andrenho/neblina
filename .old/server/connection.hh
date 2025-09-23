#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <cstdint>
#include <vector>

#include "connection_listener.hh"

class Connection {
public:
    virtual ~Connection() = default;

    void set_listener(IConnectionListener* listener) { listener_ = listener; }

    void send_data(std::vector<uint8_t> const& data) { send_data(data.data(), data.size()); }
    void send_data(std::string_view data)            { send_data((uint8_t const*) data.data(), data.size()); }
    virtual void send_data(uint8_t const* data, size_t sz) = 0;

protected:
    IConnectionListener* listener_ = nullptr;
};

#endif //CONNECTION_HH
