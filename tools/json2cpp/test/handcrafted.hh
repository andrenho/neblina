#ifndef SAMPLE_HH
#define SAMPLE_HH

#include <optional>
#include <simdjson.h>
#include <string>
#include <vector>

class HandcraftedSample {
public:
    struct Destination {
        std::string name;
        std::string address;
        std::string city;
        std::string state;
        std::optional<std::string> zip;
    };

    std::string name;
    std::vector<std::string> sku;
    double price;
    Destination ship_to;
    Destination bill_to;

    static HandcraftedSample from_file(std::string const& file_path);
    static HandcraftedSample from_string(std::string const& json_str);

private:
    static HandcraftedSample parse_json(simdjson::padded_string const& json);
    static Destination parse_destination(simdjson::simdjson_result<simdjson::ondemand::object> doc);
};

#endif //SAMPLE_HH
