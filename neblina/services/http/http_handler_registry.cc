#include "http_handler_registry.hh"

std::unordered_map<std::string, std::unique_ptr<HttpRequestHandler>> HttpHandlerRegistry::handlers_ = {};