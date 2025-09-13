#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <sys/poll.h>

#include "../service/tcp/tcp_connection.hh"

using ServiceCreateFunction = std::function<std::unique_ptr<TCPConnection>(int)>;

class TCPServer {
public:
    explicit TCPServer(ServiceCreateFunction f);
    ~TCPServer();

    void run();

protected:
    void finalize_service() { server_running_ = true; }
    static void close_connection(int fd);

    static void send_data(std::vector<uint8_t> const& data, int fd);
    static void send_data(const char* data, int fd);

    static constexpr int BUFFER_SZ = 8 * 1024;

private:
    int      listener_;         // socket fd
    bool     server_running_ = true;
    ServiceCreateFunction service_create_function_;
    std::unordered_map<int, std::unique_ptr<TCPConnection>> connections_ {};

    void handle_new_connection(std::vector<pollfd>& poll_fds);
    void handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds);

    static int  get_listener_socket();
};

#endif //TCP_SERVER_HH
