#include "tcp_server_text.hh"

void TCPServerText::new_data_available(std::vector<uint8_t> const& data, int fd)
{
    for (uint8_t byte: data) {
        if (byte != separator_) {
            buffer_.push_back(byte);
        } else {
            new_data_available(std::string(buffer_.data(), buffer_.data() + buffer_.size()), fd);
            buffer_.clear();
        }
    }
}
