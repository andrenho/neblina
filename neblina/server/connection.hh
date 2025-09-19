#ifndef CONNECTION_HH
#define CONNECTION_HH

#include <cstdint>
#include <vector>

enum class ConnectionStatus { Open, Closed };

class IConnectionListener {
public:
    virtual ~IConnectionListener() = default;
    virtual ConnectionStatus new_data_available(std::vector<uint8_t> const& data) = 0;
};

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
