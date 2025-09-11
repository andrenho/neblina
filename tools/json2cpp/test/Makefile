CPPFLAGS=-Wall -Wextra -I../../../contrib/simdjson
OBJ=../../../contrib/simdjson/simdjson.o handcrafted.o test.o

all: test-json2cpp

test-json2cpp: $(OBJ)
	g++ -o $@ $^

clean:
	rm -f $(OBJ) test-json2cpp