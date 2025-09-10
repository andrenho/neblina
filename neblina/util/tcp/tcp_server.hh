#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <cstdint>
#include <vector>
#include <sys/poll.h>

#include "util/service.hh"

class TCPServer : public Service {
public:
    TCPServer(uint16_t port, bool open_to_world=false, size_t buffer_sz=256);
    virtual ~TCPServer();

    [[noreturn]] void run() override;

    virtual void new_data_available(std::vector<uint8_t> const& data, int fd) = 0;

    static const char* name() { return "parrot_char"; }

protected:
    void send_data(std::vector<uint8_t> const& data, int fd);

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
