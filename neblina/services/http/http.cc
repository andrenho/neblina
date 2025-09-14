#include "http.hh"

#include "http.gen.inc"
#include "http_handler_registry.hh"

#include "handlers/health_request_handler.hh"

void Http::init()
{
    TCPService::init();
    HttpHandlerRegistry::add_to_registry<HealthRequestHandler>();
}

HttpConfig Http::load_config_file()
{
    create_file_if_it_doesnt_exist(config_filename(), { http, http_uncompressed_sz, sizeof http });
    return HttpConfig::from_file(config_filename());
}
