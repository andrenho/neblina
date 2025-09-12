#include "orchestrator_config.hh"
    
using namespace simdjson;
            
#include "json_support.hh"


static OrchestratorConfig parse_json(padded_string const& json)
{
    dom::parser parser;
    dom::element value = parser.parse(json);

    OrchestratorConfig obj;


    return obj;
}


OrchestratorConfig OrchestratorConfig::from_file(std::string const& file_path)
{
    padded_string json = padded_string::load(file_path);
    return parse_json(json);
}

OrchestratorConfig OrchestratorConfig::from_string(std::string const& json_str)
{
    padded_string json(json_str);
    return parse_json(json);
}
