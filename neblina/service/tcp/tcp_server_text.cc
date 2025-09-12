#include "tcp_server_text.hh"

void TCPServerText::new_data_available(std::vector<uint8_t> const& data, int fd)
{
    for (uint8_t byte: data) {
        buffer_ += (char) byte;
        size_t buf_len = buffer_.size();
        if (buf_len >= sep_len_ && buffer_.substr(buf_len - sep_len_, sep_len_) == separator_) {
            buffer_ = buffer_.substr(0, buf_len - sep_len_);
            new_data_available(buffer_, fd);
            buffer_.clear();
        }
    }
}
