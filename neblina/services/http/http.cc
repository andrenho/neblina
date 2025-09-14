#include "http.hh"

void HttpConnection::new_data_available(std::string_view data)
{
    // TODO - check exceptions
    current_http_request << data;
}
