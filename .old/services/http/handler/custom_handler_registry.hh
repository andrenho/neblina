#ifndef HTTP_HANDLER_REGISTRY_HH
#define HTTP_HANDLER_REGISTRY_HH

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "http_request_handler.hh"

class HttpHandlerRegistry {
public:
    template <typename... Ts>
    static void add_to_registry()
    {
        (add_item_to_registry<Ts>(), ...);
    }

    static std::unique_ptr<HttpRequestHandler> create_unique_ptr(std::string const& name) { return handlers_.at(name)(); }

private:
    template <typename T>
    static void add_item_to_registry()
    {
        handlers_[T::name] = []() { return std::make_unique<T>(); };
    }

    static std::unordered_map<std::string, std::function<std::unique_ptr<HttpRequestHandler>()>> handlers_;
};

#endif //HTTP_HANDLER_REGISTRY_HH
