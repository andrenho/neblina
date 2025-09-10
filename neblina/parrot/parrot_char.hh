#ifndef PARROT_CHAR_HH
#define PARROT_CHAR_HH

#include "util/tcp/tcp_server.hh"

class ParrotChar : public TCPServer {
public:
    ParrotChar(int port, bool open_to_world) : TCPServer(port, open_to_world) {}

    void new_data_available(std::vector<uint8_t> const& data, int fd) override { send_data(data, fd); }
};

#endif //PARROT_CHAR_HH
