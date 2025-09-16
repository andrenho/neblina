#include "http.hh"

#include "http_handler_registry.hh"

#include "handlers/health_request_handler.hh"
#include "handlers/redirect_request_handler.hh"
#include "handlers/static_dir_request_handler.hh"

void Http::init()
{
    TCPService::init();
    HttpHandlerRegistry::add_to_registry<HealthRequestHandler>();

    // create list of request handers
    for (auto const& rt: config_.routes) {
        try {
            std::unique_ptr<HttpRequestHandler> handler;
            if (rt.handler) {
                handler = HttpHandlerRegistry::create_unique_ptr(*rt.handler);
            } else if (rt.serve_static_dir) {
                handler = std::make_unique<StaticDirRequestHandler>(*rt.serve_static_dir);
            } else if (rt.redirect) {
                handler = std::make_unique<RedirectRequestHandler>(*rt.redirect);
            }
            if (handler) {
                routes_.emplace_back(HttpRoute {
                    .regex = std::regex(rt.path),
                    .handler = std::move(handler),
                });
            }
        } catch (std::out_of_range&) {
            throw std::runtime_error(std::format("Handler '{}' is not available.", *rt.handler));
        } catch (std::regex_error&) {
            throw std::runtime_error(std::format("Path '{}' is not a valid regex expression.", rt.path));
        }
    }
}

HttpConfig Http::load_config_file()
{
    return HttpConfig::from_file(config_filename());
}
