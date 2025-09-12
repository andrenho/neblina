#include "sample.hh"

#include <cassert>

int main()
{
    Sample sample = Sample::from_file("sample.json");

    assert(sample.first_name == "John");
    assert(sample.is_alive);
    assert(sample.age == 27);
    assert(sample.address.city == "New York");
    assert(sample.phone_numbers.size() == 2);
    assert(sample.phone_numbers.at(0).type == "home");
    assert(!sample.phone_numbers.at(0).postal_code);
    assert(sample.phone_numbers.at(1).postal_code);
    assert(*sample.phone_numbers.at(1).postal_code == "10021-3100");
    assert(sample.children.size() == 3);
    assert(sample.children.at(1) == "Thomas");
    assert(!sample.spouse);
}
