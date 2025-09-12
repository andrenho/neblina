#ifndef REGISTRY_HH
#define REGISTRY_HH

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include "service.hh"

using ServiceRegistry = std::unordered_map<std::string_view, std::function<std::unique_ptr<Service>()>>;

template <typename T>
void register_native_service(ServiceRegistry& services)
{
    services[T::name] = [](){ return std::make_unique<T>(); };
}

template <typename... Ts>
void register_native_services(ServiceRegistry& services)
{
    (register_native_service<Ts>(services), ...);
}


#endif //REGISTRY_HH
