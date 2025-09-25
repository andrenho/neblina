#include "example.h"

#include <assert.h>
#include <string.h>

int main()
{
    Example example;
    assert(example_json_load("example.json", &example) == J_OK);

    // assert(strcmp(example.name, "John Smith") == 0);
    assert(example.age == 107);
    assert(example.height == 3.14);
    assert(!example.gender);
    assert(example.optional == INT_NULL);
    assert(example.items_sz == 2);
    assert(example.items[0] == 1);
    assert(example.items[1] == 2);
    assert(example.items[2] == 3);
    /*
    assert(example.obj_sz == 2);
    assert(strcmp(example.obj[0].name, "a") == 0);
    assert(example.obj[0].detail.is_null);
    assert(strcmp(example.obj[1].name, "b") == 0);
    assert(!example.obj[1].detail.is_null);
    assert(example.obj[1].detail.number == 4);
    */
}