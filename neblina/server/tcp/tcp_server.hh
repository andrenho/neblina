#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <sys/poll.h>

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "tcp_connection.hh"
#include "server/server.hh"

class TCPServer : public Server {
public:
    TCPServer();
    ~TCPServer() override;

    void iteration() override;

protected:
    virtual std::unique_ptr<TCPConnection> new_connection(int fd, std::string const& host, std::string const& port) const;

private:
    int                 socket_fd_;        // socket fd
    std::vector<pollfd> poll_fds {1};
    std::unordered_map<int, std::unique_ptr<TCPConnection>> connections_ {};

    void handle_new_connection(std::vector<pollfd>& poll_fds);
    void handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds);

    static int get_listener_socket();
};

#endif //TCP_SERVER_HH
