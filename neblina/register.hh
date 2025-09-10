#ifndef REGISTER_HH
#define REGISTER_HH

#include <functional>
#include <memory>
#include <unordered_map>
#include <string>

#include "arguments.hh"
#include "service/service.hh"

using ServiceRegistry = std::unordered_map<std::string, std::function<std::unique_ptr<Service>(Arguments&)>>;

template <typename T>
void register_native_service(ServiceRegistry& services)
{
    services[T::name()] = [](Arguments& args){ return std::make_unique<T>(args); };
}

template <typename... Ts>
void register_native_services(ServiceRegistry& services)
{
    (register_native_service<Ts>(services), ...);
}


#endif //REGISTER_HH
