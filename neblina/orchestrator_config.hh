#ifndef ORCHESTRATORCONFIG_HH
#define ORCHESTRATORCONFIG_HH

#include <optional>
#include <string>
#include <vector>

#include <simdjson.h>

class OrchestratorConfig {
public:


    static OrchestratorConfig from_file(std::string const& file_path);
    static OrchestratorConfig from_string(std::string const& json_str);
};

#endif //ORCHESTRATORCONFIG_HH
