#ifndef TCP_CONNECTION_BY_LINE_HH
#define TCP_CONNECTION_BY_LINE_HH
#include <string>

#include "tcp_connection.hh"

class TCPConnectionText : public TCPConnection {
public:
    explicit TCPConnectionText(int fd, std::string const& separator="\r\n")
        : TCPConnection(fd), separator_(separator), sep_len_(separator.size())
    {
        buffer_.reserve(16 * 1024);
    }

    void new_data_available(std::vector<uint8_t> const& data) override;

    virtual void new_data_available(std::string_view data) = 0;

private:
    std::string buffer_ {};
    const std::string separator_;
    const size_t      sep_len_;
};

#endif //TCP_CONNECTION_BY_LINE_HH
