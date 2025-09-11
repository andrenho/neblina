#include "handcrafted.hh"

#include <simdjson.h>
using namespace simdjson;

#include "../json_support.hh"

template <>
HandcraftedSample::Destination extract_value(dom::element const& value)
{
    HandcraftedSample::Destination obj;
    obj.name = require<std::string>(value, "name");
    obj.state = require<std::string>(value, "state");
    obj.zip = optional<std::string>(value, "zip");
    return obj;
}

HandcraftedSample parse_json(padded_string const& json)
{
    dom::parser parser;
    dom::element value = parser.parse(json);

    HandcraftedSample obj;
    obj.name = require<std::string>(value, "name");
    obj.sku = require<std::vector<std::string>>(value, "sku");
    obj.price = require<double>(value, "price");
    obj.ship_to = require<HandcraftedSample::Destination>(value, "shipTo");
    obj.bill_to = require<HandcraftedSample::Destination>(value, "billTo");
    return obj;
}

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

