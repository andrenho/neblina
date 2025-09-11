#ifndef PARROT_CHAR_HH
#define PARROT_CHAR_HH

#include "service/tcp/tcp_server.hh"

class ParrotChar : public TCPServer {
public:
    explicit ParrotChar(Arguments const& args) : TCPServer(args, 23456, false) {}

    void new_data_available(std::vector<uint8_t> const& data, int fd) override { send_data(data, fd); }

    static const char* name() { return "parrot_char"; }
};

#endif //PARROT_CHAR_HH
