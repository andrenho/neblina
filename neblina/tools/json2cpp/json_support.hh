#ifndef JSON_SUPPORT_HH
#define JSON_SUPPORT_HH

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
    if (value.error() || value.is_null())
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

#endif //JSON_SUPPORT_HH
