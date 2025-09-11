#include "handcrafted.hh"
#include "sample.hh"

#include <assert.h>

int main()
{
    HandcraftedSample hsample = HandcraftedSample::from_file("sample.json");

    assert(hsample.name == "John Smith");
    assert(hsample.sku.size() == 2);
    assert(hsample.sku.at(0) == "20223");
    assert(hsample.sku.at(1) == "23312");
    assert(hsample.price == 23.95);
    assert(hsample.ship_to.name == "Jane Smith");
    assert(hsample.bill_to.name == "John Smith");
    assert(hsample.ship_to.zip == "12345");
    assert(!hsample.bill_to.zip.has_value());
}
