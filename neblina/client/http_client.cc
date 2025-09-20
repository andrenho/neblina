#include "http_client.hh"

#include "tcp_client.hh"
#include "services/http/types/http_exceptions.hh"
#include "util/string.hh"

HttpResponse http_client_request(HttpRequest const& request, int port)
{
    if (!request.headers.contains("Host"))
        throw BadRequestException();

    TCPClient client(*request.headers.at("Host"), port);
    client.send(request.to_string());
    size_t content_length = 0;
    std::string str_response;
    for (;;) {
        std::string line = client.recv_line();
        str_response += line + "\r\n";
        if (to_upper(line).starts_with("CONTENT-LENGTH"))
            content_length = std::stoull(line.substr(line.find(':') + 1));
        if (line.empty()) {
            if (content_length > 0) {
                str_response += client.recv(content_length);
                break;
            }
        }
    }

    return HttpResponse::from_string(str_response);
}
