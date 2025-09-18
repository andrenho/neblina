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

    void run(std::function<bool()> should_exit);

protected:
    void finalize_service() { server_running_ = true; }

    static constexpr int BUFFER_SZ = 8 * 1024;

private:
    int               listener_;         // socket fd
    bool              server_running_ = true;
    TCPService const* service_;
    std::unordered_map<int, std::unique_ptr<class TCPConnection>> connections_ {};

    void handle_new_connection(std::vector<pollfd>& poll_fds);
    void handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds);

    static int  get_listener_socket();
};

#endif //TCP_SERVER_HH
