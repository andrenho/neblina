#include "http.hh"

#include "http_handler_registry.hh"

#include "handlers/health_request_handler.hh"

void Http::init()
{
    TCPService::init();
    HttpHandlerRegistry::add_to_registry<HealthRequestHandler>();
}

HttpConfig Http::load_config_file()
{
    return HttpConfig::from_file(config_filename());
}
