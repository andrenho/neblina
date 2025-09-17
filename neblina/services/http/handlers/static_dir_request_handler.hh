#ifndef STATIC_DIR_REQUEST_HANDLER_HH
#define STATIC_DIR_REQUEST_HANDLER_HH

#include "static_request_handler.hh"

class StaticDirRequestHandler : public StaticRequestHandler {
public:
    explicit StaticDirRequestHandler(std::string const& dir_name) : dir_name_(dir_name) {}

protected:
    HttpResponse file_contents(const std::vector<std::string>& file_path, std::string const& cached_etag) const override;

private:
    std::string dir_name_;
};

#endif //STATIC_DIR_REQUEST_HANDLER_HH
