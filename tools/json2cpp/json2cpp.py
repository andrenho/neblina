#!/usr/bin/env python3

import sys
import re
import json

#
# functions
#

def to_snake_case(text):
    return re.sub(r'(?<!^)(?=[A-Z])', '_', text).lower()

def translate_type(type: str):
    new_type = None
    if type.startswith('string'):
        new_type = 'std::string'
    else:
        new_type = type.replace('[]', '').replace('*', '')
    if '[]' in type:
        new_type = 'std::vector<' + new_type + '>'
    if '*' in type:
        new_type = 'std::optional<' + new_type + '>'
    return new_type

#
# main
#

if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + "schema.json ClassName")
schema = sys.argv[1]
classname = sys.argv[2]
cpp_filename = to_snake_case(classname)
uclass = classname.upper()

with open(schema, "r") as file:
    data = json.load(file)

#
# header
#

with open(cpp_filename + ".hh", "w") as f:
    f.write(f'''#ifndef {uclass}_HH
#define {uclass}_HH

#include <optional>
#include <string>
#include <vector>

#include <simdjson.h>

class {classname} {{
public:
''')

    types_already_added = { "string", "double", "int", "bool" }
    additional_types = []
    def add_types(code, struct, level=4):
        for var in struct:
            type = struct[var]
            if type.replace('[]','').replace('*','') not in types_already_added:
                types_already_added.add(type)
                additional_types.append(type)
                new_struct = [' ' * level + 'struct ' + type + ' {\n']
                add_types(new_struct, data[type], level + 4)
                new_struct.append(' ' * level + '};\n\n')
                for i in reversed(new_struct):
                    code.insert(0, i)
            code.append(' ' * level + translate_type(struct[var]) + " " + to_snake_case(var) + ";\n")

    code = []
    add_types(code, data['main'])

    f.write(''.join(code))
    f.write(f'''
    static {classname} from_file(std::string const& file_path);
    static {classname} from_string(std::string const& json_str);
}};

#endif //{uclass}_HH
''')

#
# source
#

def generate_fields(type):
    r = []
    for var in data[type]:
        f = 'optional' if '*' in data[type][var] else 'require'
        ctype = translate_type(data[type][var].replace('*',''))
        ns = classname + '::' if data[type][var].replace('*','').replace('[]', '') not in  ['string', 'double', 'int', 'bool'] else ''
        r.append(f'    obj.{to_snake_case(var)} = {f}<{ns}{ctype}>(value, "{var}");')
    return '\n'.join(r) + "\n"

with open(cpp_filename + ".cc", "w") as f:
    f.write(f'''#include "{cpp_filename}.hh"
    
using namespace simdjson;
            
#include "json_support.hh"

''')

    for type in additional_types:
        f.write(f'''
template <>
{classname}::{type} extract_value(dom::element const& value)
{{
    {classname}::{type} obj;
''')
        f.write(generate_fields(type))
        f.write(f'''    return obj;
}}

''')

    f.write(f'''static {classname} parse_json(padded_string const& json)
{{
    dom::parser parser;
    dom::element value = parser.parse(json);

    {classname} obj;
''')
    f.write(generate_fields('main'))
    f.write(f'''    return obj;
}}

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

''')