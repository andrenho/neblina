#include "handcrafted.hh"

#include <simdjson.h>
using namespace simdjson;

HandcraftedSample HandcraftedSample::from_file(std::string const& file_path)
{
    padded_string json = padded_string::load(file_path);
    return parse_json(json);
}

HandcraftedSample HandcraftedSample::from_string(std::string const& json_str)
{
    padded_string json(json_str);
    return parse_json(json);
}

HandcraftedSample HandcraftedSample::parse_json(padded_string const& json)
{
    ondemand::parser parser;
    ondemand::document doc = parser.iterate(json);

    HandcraftedSample obj;
    obj.name = doc["name"].get_string().take_value();
    for (auto sku: doc["sku"].get_array())
        obj.sku.emplace_back(doc["sku"].get_string().take_value());
    obj.price = doc["price"].get_double();
    return obj;
}
