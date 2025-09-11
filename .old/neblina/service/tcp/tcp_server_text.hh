#ifndef TCP_SERVER_TEXT_HH
#define TCP_SERVER_TEXT_HH

#include <vector>
#include <string>

#include "tcp_server.hh"

class TCPServerText : public TCPServer {
public:
    TCPServerText(struct Arguments const& args, uint16_t port, bool open_to_world=false, size_t buffer_sz=256, std::string const& separator="\r\n")
        : TCPServer(args, port, open_to_world, buffer_sz), separator_(separator), sep_len_(separator.size())
    {
        buffer_.reserve(buffer_sz);
    }

protected:
    virtual void new_data_available(std::string const& text, int fd) = 0;
    void new_data_available(std::vector<uint8_t> const& data, int fd) override;

private:
    std::string       buffer_;
    const std::string separator_;
    const size_t      sep_len_;
};

#endif //TCP_SERVER_TEXT_HH
