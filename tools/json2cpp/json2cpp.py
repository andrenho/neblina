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
    def add_types(code, struct, level=4):
        for var in struct:
            type = struct[var]
            if type.replace('[]','').replace('*','') not in types_already_added:
                types_already_added.add(type)
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

with open(cpp_filename + ".cc", "w") as f:
    pass
