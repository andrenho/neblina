#include "connection_listener.hh"

ConnectionStatus IConnectionListener::new_data_available(std::vector<uint8_t> const& data)
{
    ConnectionStatus r = ConnectionStatus::Open;

    r = new_data(data);
    if (r == ConnectionStatus::Closed)
        return r;

    if (sep_len_ != 0) {
        for (uint8_t byte: data) {
            buffer_ += (char) byte;
            size_t buf_len = buffer_.size();
            if (buf_len >= sep_len_ && buffer_.substr(buf_len - sep_len_, sep_len_) == separator_) {
                buffer_ = buffer_.substr(0, buf_len - sep_len_);
                r = new_line(buffer_);
                if (r == ConnectionStatus::Closed)
                    return r;
                buffer_.clear();
            }
        }
    }

    return r;
}
