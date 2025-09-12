#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <cstdint>
#include <vector>
#include <sys/poll.h>

#include "service/service.hh"

class TCPServer : public Service {
public:
    TCPServer();
    ~TCPServer() override;

    void run() override;

protected:
    virtual void new_data_available(std::vector<uint8_t> const& data, int fd) = 0;

    void finalize_service() { server_running_ = true; }
    static void close_connection(int fd);

    static void send_data(std::vector<uint8_t> const& data, int fd);
    static void send_data(const char* data, int fd);

    static constexpr int BUFFER_SZ = 8 * 1024;

private:
    int      listener_;         // socket fd
    bool     server_running_ = true;

    void handle_new_connection(std::vector<pollfd>& poll_fds) const;
    void handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds);

    static int  get_listener_socket();
};

#endif //TCP_SERVER_HH
