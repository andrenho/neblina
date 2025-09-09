#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <cstdint>
#include <vector>
#include <sys/poll.h>

class TCPServer {
public:
    TCPServer(uint16_t port, bool open_to_world=false, size_t buffer_sz=256);
    ~TCPServer();

    [[noreturn]] void run();

private:
    uint16_t port_;
    bool     open_to_world_;
    int      listener_;         // socket fd
    bool     server_running_ = true;
    size_t   buffer_sz_;

    void handle_new_connection(std::vector<pollfd>& poll_fds);
    void handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds);

    int  get_listener_socket();
};

#endif //TCP_SERVER_HH
