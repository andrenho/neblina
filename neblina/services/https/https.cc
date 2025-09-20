#include "https.hh"

#include "server/ssl/ssl_server.hh"
#include "services/http/handlers/proxy_request_handler.hh"

// TODO - we're reading 3 times the config file

Https::Https()
    : CommunicationService(std::make_unique<SSLServer>(
          args().data_dir / "https" / HttpsConfig::from_file(config_filename()).public_key,
          args().data_dir / "https" / HttpsConfig::from_file(config_filename()).private_key
      )),
     config_(HttpsConfig::from_file(config_filename()))
{
    routes_.emplace_back(std::regex("/(.*)"), std::make_unique<ProxyRequestHander>("https", "localhost", config_.proxy_port));
}
