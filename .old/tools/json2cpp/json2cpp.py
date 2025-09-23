#!/usr/bin/env python3

import sys
import re
import json

json_types = ["string", "double", "int", "bool"]

#
# functions
#

def to_snake_case(text):
    return re.sub(r'(?<!^)(?=[A-Z])', '_', text).lower()

def translate_type(type: str, classname=""):
    new_type = None
    if type.startswith('string'):
        new_type = 'std::string'
    else:
        new_type = type.replace('[]', '').replace('*', '')
    if classname != '' and type.replace('[]', '') not in json_types:
        new_type = classname + "::" + new_type
    if '[]' in type:
        new_type = 'std::vector<' + new_type + '>'
    if '*' in type:
        new_type = 'std::optional<' + new_type + '>'
    return new_type

#
# main
#

if len(sys.argv) != 3 and len(sys.argv) != 4:
    print("Usage: " + sys.argv[0] + "schema.json ClassName [output_directory]")

schema_filename = sys.argv[1]
classname = sys.argv[2]
cpp_filename = to_snake_case(classname)
upper_classname = classname.upper()
output_dir = sys.argv[3] + '/' if len(sys.argv) >=4 else ""

with open(schema_filename, "r") as file:
    data = json.load(file)

#
# header
#

# create list of C++ types

types_already_added = { "string", "double", "int", "bool" }
additional_types = []
def add_types(code, struct, level=4):
    for var in struct:
        type = struct[var]
        clean_type = type.replace('[]','').replace('*','')
        if clean_type not in types_already_added:
            types_already_added.add(clean_type)
            additional_types.append(clean_type)
            new_struct = [' ' * level + 'struct ' + clean_type + ' {\n']
            add_types(new_struct, data[clean_type], level + 4)
            new_struct.append(' ' * level + '};\n\n')
            for i in reversed(new_struct):
                code.insert(0, i)
        code.append(' ' * level + translate_type(struct[var]) + " " + to_snake_case(var) + ";\n")
code = []
add_types(code, data['main'])

# HH header template

header = f'''#ifndef {upper_classname}_HH
#define {upper_classname}_HH

#include <optional>
#include <string>
#include <vector>

#include <simdjson.h>

class {classname} {{
public:
{''.join(code)}

    static {classname} from_file(std::string const& file_path);
    static {classname} from_string(std::string const& json_str);
private:
    {classname}() = default;
    static {classname} parse_json(simdjson::padded_string const& json);
}};

#endif //{upper_classname}_HH
'''

# save to file

with open(output_dir + cpp_filename + ".hh", "w") as f:
    f.write(header)


#
# source
#

def generate_fields(type):
    r = []
    for var in data[type]:
        f = 'optional' if '*' in data[type][var] else 'require'
        ctype = translate_type(data[type][var].replace('*',''), classname)
        r.append(f'    obj.{to_snake_case(var)} = {f}<{ctype}>(value, "{var}");')
    return '\n'.join(r)

# additional types parsing function

def additional_types_parsing_function(type):
    return f'''template<>
{classname}::{type} extract_value(dom::element const& value)
{{
    {classname}::{type} obj;
{generate_fields(type)}
    return obj;
}}'''

main_parsing_function = f'''{classname} {classname}::parse_json(padded_string const& json)
{{
    dom::parser parser;
    dom::element value = parser.parse(json);

    {classname} obj;
{generate_fields('main')}
    return obj;
}}'''

# C++ source

source = f'''#include "{cpp_filename}.hh"
    
using namespace simdjson;
            
#include "json_support.hh"

{"\n\n".join(map(additional_types_parsing_function, additional_types))}
{main_parsing_function}


{classname} {classname}::from_file(std::string const& file_path)
{{
    padded_string json = padded_string::load(file_path);
    return parse_json(json);
}}

{classname} {classname}::from_string(std::string const& json_str)
{{
    padded_string json(json_str);
    return parse_json(json);
}}
'''

with open(output_dir + cpp_filename + ".cc", "w") as f:
    f.write(source)