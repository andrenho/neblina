#ifndef TCP_CLIENT_HH
#define TCP_CLIENT_HH

#include <string>

class TCPClient {
public:
    TCPClient(std::string const& destination, int port);
    ~TCPClient();

    std::string recv(size_t nbytes);
    std::string recv_line(std::string const& separator="\r\n");

    void send(std::string const& data);

    [[nodiscard]] bool connection_closed() const { return connection_closed_; }

private:
    int         fd_ = 0;
    std::string buffer_;
    bool        connection_closed_ = false;
};


#endif //TCP_CLIENT_HH
