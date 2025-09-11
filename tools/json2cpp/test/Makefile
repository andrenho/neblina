CPPFLAGS=-Wall -Wextra -I../../../contrib/simdjson -std=c++23 -ggdb -O0
OBJ=../../../contrib/simdjson/simdjson.o handcrafted.o test.o sample.o

all: test-json2cpp

sample.cc: sample_schema.json ../json2cpp.py
	../json2cpp.py sample_schema.json Sample

test-json2cpp: $(OBJ)
	g++ -o $@ $^

clean:
	rm -f $(OBJ) test-json2cpp sample.hh sample.cc