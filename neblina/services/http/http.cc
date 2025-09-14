#include "http.hh"

void HttpConnection::new_data_available(std::string_view data)
{
    current_http_request << data;
}
