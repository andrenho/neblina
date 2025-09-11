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

static simdjson_result<ondemand::value> validate(ondemand::value doc, const char* field)
{
    auto const& value = doc[field];
    if (value.error())
        throw std::runtime_error(std::string("Mandatory field '") + field + "not found in JSON.");
    return value;
}

template <typename T>
static std::optional<T> opt(ondemand::value doc, const char* field)
{
    auto value = doc[field];
    if (value.error())
        return {};
    return value;
}

namespace json {

template<typename T>
concept IsVector = requires {
    typename T::value_type;
    requires std::is_same_v<T, std::vector<typename T::value_type, typename T::allocator_type>>;
};

template<typename T>
concept NotVector = !IsVector<T>;

template <typename T>
static T extract_value(auto&& value)
{
    if constexpr(std::is_same_v<T, std::string>)
        return std::string(value.get_string().take_value());
    else if constexpr(std::is_same_v<T, double>)
        return value.get_double();
    else if constexpr(std::is_same_v<T, int>)
        return value.get_int64();
    else if constexpr(std::is_same_v<T, bool>)
        return value.get_bool();
    return {};
}

template <NotVector T>
static std::optional<T> optional(auto&& doc, const char* field)
{
    auto value = doc[field];
    if (value.error())
        return {};
    return extract_value<T>(value);
}

template <IsVector T>
static std::optional<T> optional(auto&& doc, const char* field)
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
static T require(auto&& doc, const char* field)
{
    auto opt = optional<T>(doc, field);
    if (!opt)
        throw std::runtime_error(std::string("Mandatory field '") + field + "not found in JSON.");
    return opt.value();
}

}

HandcraftedSample HandcraftedSample::parse_json(padded_string const& json)
{
    ondemand::parser parser;
    ondemand::document doc = parser.iterate(json);

    HandcraftedSample obj;
    obj.name = json::require<std::string>(doc, "name");
    obj.sku = json::require<std::vector<std::string>>(doc, "sku");
    obj.price = json::require<double>(doc, "price");
    obj.bill_to = parse_destination(doc["billTo"].get_object());
    obj.ship_to = parse_destination(doc["shipTo"].get_object());
    return obj;
}

HandcraftedSample::Destination HandcraftedSample::parse_destination(simdjson_result<ondemand::object> doc)
{
    Destination obj;
    obj.name = json::require<std::string>(doc, "name");
    obj.state = json::require<std::string>(doc, "state");
    obj.zip = json::optional<std::string>(doc, "zip");
    return obj;
}
