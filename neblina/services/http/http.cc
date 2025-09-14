#include "http.hh"

#include "util/http/http_response.hh"

void HttpConnection::new_data_available(std::string_view data)
{
    try {
        current_http_request << data;
    } catch (HttpRequest::BadRequest&) {
        send_data(HttpResponse(400, "The client sent a request that the server did not understand.").to_string());
        close_connection();
    } catch (HttpRequest::DataAfterBody&) {
        send_data(HttpResponse(413, "Request body too long").to_string());
        close_connection();
    } catch (HttpRequest::UnsupportedHttpVersion&) {
        send_data(HttpResponse(505, "The HTTP version requested is not supported by this server.").to_string());
        close_connection();
    } catch (HttpRequest::NotSupportedYet& e) {
        send_data(HttpResponse(500, std::string(e.what()) + ": not supported yet by this server.").to_string());
        close_connection();
    }

    if (current_http_request.complete())
        parse_request(current_http_request);
}

void HttpConnection::parse_request(HttpRequest request)
{
    // TODO
}
