#include "http_router.hh"

#include "http_router.gen.inc"

HttpRouterConfig HttpRouter::load_config_file()
{
    create_file_if_it_doesnt_exist(config_filename(), { http_router, http_router_uncompressed_sz, sizeof http_router });
    return HttpRouterConfig::from_file(config_filename());
}
