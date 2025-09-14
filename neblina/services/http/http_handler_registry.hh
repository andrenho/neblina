#ifndef HTTP_HANDLER_REGISTRY_HH
#define HTTP_HANDLER_REGISTRY_HH

#include <memory>
#include <string>
#include <unordered_map>

#include "http_request_handler.hh"

class HttpHandlerRegistry {
public:
    template <typename T>
    static void add_to_registry()
    {
        handlers_[T::name] = std::make_unique<T>();
    }

    static HttpRequestHandler& at(std::string const& name) { return *handlers_.at(name).get(); }

private:
    static std::unordered_map<std::string, std::unique_ptr<HttpRequestHandler>> handlers_;
};

#endif //HTTP_HANDLER_REGISTRY_HH
