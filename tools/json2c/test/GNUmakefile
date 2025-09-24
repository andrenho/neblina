all: test_json

CPPFLAGS = -std=c17 -Wall -Wextra -g -O0 -I. -I../../../src/contrib -I../../../src
OBJ = main.o example.o ../../../src/util/json_support.o

test_json: $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJ) test_json
