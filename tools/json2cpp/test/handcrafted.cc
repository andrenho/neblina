#include "handcrafted.hh"

#include <simdjson.h>
using namespace simdjson;

template<typename T>
concept IsVector = requires {
    typename T::value_type;
    requires std::is_same_v<T, std::vector<typename T::value_type, typename T::allocator_type>>;
};

template<typename T>
concept NotVector = !IsVector<T>;

template <typename T>
T extract_value(dom::element const& value)
{
    if constexpr(std::is_same_v<T, std::string>)
        return std::string(value.get_string().take_value());
    else if constexpr(std::is_same_v<T, double>)
        return value.get_double();
    else if constexpr(std::is_same_v<T, int>)
        return value.get_int64();
    else if constexpr(std::is_same_v<T, bool>)
        return value.get_bool();
    throw std::runtime_error("JSON parsing not implemented for this type.");
}


template <NotVector T>
std::optional<T> optional(dom::element const& doc, const char* field)
{
    auto value = doc[field];
    if (value.error())
        return {};
    return extract_value<T>(value.value());
}

template <IsVector T>
std::optional<T> optional(dom::element const& doc, const char* field)
{
    auto value = doc[field];
    if (value.error())
        return {};

    std::vector<typename T::value_type> items;
    for (auto item: value.get_array())
        items.emplace_back(extract_value<typename T::value_type>(item));
    return items;
}

template <typename T>
T require(dom::element const& doc, const char* field)
{
    auto opt = optional<T>(doc, field);
    if (!opt)
        throw std::runtime_error(std::string("Mandatory field '") + field + "not found in JSON.");
    return opt.value();
}

template <>
HandcraftedSample::Destination extract_value(dom::element const& value)
{
    HandcraftedSample::Destination obj;
    obj.name = require<std::string>(value, "name");
    obj.state = require<std::string>(value, "state");
    obj.zip = optional<std::string>(value, "zip");
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

HandcraftedSample HandcraftedSample::parse_json(padded_string const& json)
{
    dom::parser parser;
    dom::element value = parser.parse(json);

    HandcraftedSample obj;
    obj.name = require<std::string>(value, "name");
    obj.sku = require<std::vector<std::string>>(value, "sku");
    obj.price = require<double>(value, "price");
    obj.ship_to = require<Destination>(value, "shipTo");
    obj.bill_to = require<Destination>(value, "billTo");
    return obj;
}

