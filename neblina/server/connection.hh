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

protected:
    IConnectionListener* listener_ = nullptr;
};

#endif //CONNECTION_HH
