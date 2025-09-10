#ifndef TCP_SERVER_HH
#define TCP_SERVER_HH

#include <cstdint>
#include <vector>
#include <sys/poll.h>

#include "arguments.hh"
#include "service/service.hh"

class TCPServer : public Service {
public:
    TCPServer(Arguments const& args, uint16_t port, bool open_to_world=false, size_t buffer_sz=256);
    ~TCPServer() override;

    void run() override;

    virtual void new_data_available(std::vector<uint8_t> const& data, int fd) = 0;

protected:
    static void send_data(std::vector<uint8_t> const& data, int fd);

    void finalize_service() { server_running_ = true; }

private:
    uint16_t port_;
    bool     open_to_world_;
    int      listener_;         // socket fd
    bool     server_running_ = true;
    size_t   buffer_sz_;

    void handle_new_connection(std::vector<pollfd>& poll_fds) const;
    void handle_new_data(pollfd const& pfd, std::vector<pollfd>& poll_fds);

    int  get_listener_socket() const;
};

#endif //TCP_SERVER_HH
