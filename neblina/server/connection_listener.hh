#ifndef CONNECTION_LISTENER_HH
#define CONNECTION_LISTENER_HH

#include <cstdint>
#include <string>
#include <vector>

enum class ConnectionStatus { Open, Closed };

class IConnectionListener {
public:
    IConnectionListener() = default;
    explicit IConnectionListener(std::string const& separator)
        : separator_(separator), sep_len_(separator_.size()) { buffer_.reserve(16 * 1024); }

    virtual ~IConnectionListener() = default;

    ConnectionStatus new_data_available(std::vector<uint8_t> const& data);

    virtual ConnectionStatus new_data(std::vector<uint8_t> const& data) { return ConnectionStatus::Open; }
    virtual ConnectionStatus new_line(std::string_view data) { return ConnectionStatus::Open; }

private:
    std::string buffer_ {};
    std::string separator_ {};
    size_t      sep_len_ = 0;
};

#endif //CONNECTION_LISTENER_HH
