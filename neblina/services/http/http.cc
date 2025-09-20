#include "http.hh"

#include "handler/custom_handler_registry.hh"

#include "handlers/health_request_handler.hh"
#include "handlers/proxy_request_handler.hh"
#include "handlers/redirect_request_handler.hh"
#include "handlers/static_dir_request_handler.hh"
#include "util/exceptions/non_recoverable_exception.hh"

// edit this list to add a new native service
#define HANDLERS HealthRequestHandler

Http::Http()
    : CommunicationService(std::make_unique<TCPServer>()),
      config_(HttpConfig::from_file(config_filename()))
{
}

void Http::init()
{
    CommunicationService::init();

    HttpHandlerRegistry::add_to_registry<HealthRequestHandler>();

    // create list of HTTP request handlers
    for (auto const& rt: config_.routes) {
        try {
            std::unique_ptr<HttpRequestHandler> handler;

            if (rt.handler) {
                // it's a custom handler - runs a custom C++ HTTP application
                handler = HttpHandlerRegistry::create_unique_ptr(*rt.handler);

            } else if (rt.serve_static_dir) {
                // it's a static handler - it serves webpages from a directory in the disk
                handler = std::make_unique<StaticDirRequestHandler>(*rt.serve_static_dir);

            } else if (rt.redirect) {
                // it's a redirect handler - it always responds with 301
                handler = std::make_unique<RedirectRequestHandler>(*rt.redirect);

            } else if (rt.proxy_host) {
                // it's a proxy handers - proxies to the next server
                if (!rt.proxy_port)
                    throw NonRecoverableException("If proxyHost is used, then proxyPort is mandatory");
                handler = std::make_unique<ProxyRequestHander>("http", *rt.proxy_host, *rt.proxy_port);
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
