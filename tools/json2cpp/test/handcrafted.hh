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
};

#endif //SAMPLE_HH
