#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <sys/poll.h>

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

class TCPServer {
public:
    explicit TCPServer(class TCPService const* service);
    ~TCPServer();

    void run(std::function<bool()> const& should_exit);

private:
    TCPService const* service_;          // used to create new connections
    int               socket_fd_;        // socket fd
    std::unordered_map<int, std::unique_ptr<class TCPConnection>> connections_ {};

    void handle_new_connection(std::vector<pollfd>& poll_fds);
    void handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds);

    static int get_listener_socket();

    static constexpr int BUFFER_SZ = 8 * 1024;
};

#endif //TCP_SERVER_HH
