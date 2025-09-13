#ifndef HTTP_HH
#define HTTP_HH
#include "../tcp/tcp_server_text.hh"

class Http : public TCPServerText {
public:
    static constexpr std::string_view name = "http";

protected:
    void new_data_available(std::string const& text, int fd) override;

private:
    static void it_works(int fd);
};

#endif //HTTP_HH
