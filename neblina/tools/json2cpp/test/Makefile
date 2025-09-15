SIMDJSON_PATH=../../../neblina/contrib/simdjson
CPPFLAGS=-Wall -Wextra -I$(SIMDJSON_PATH) -I.. -std=c++23 -ggdb -O0
OBJ=$(SIMDJSON_PATH)/simdjson.o test.o sample.o

all: test-json2cpp

test.o: sample.o

sample.cc: sample_schema.json ../json2cpp.py
	../json2cpp.py sample_schema.json Sample

test-json2cpp: $(OBJ)
	g++ -o $@ $^

clean:
	rm -f $(OBJ) test-json2cpp sample.hh sample.cc