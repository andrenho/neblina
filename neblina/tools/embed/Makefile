CPPFLAGS=-Wall -Wextra -g -O0 -I../../contrib/miniz
CXXFLAGS=-std=c++23

OBJ = main.o ../../contrib/miniz/miniz.o ../../file/gz.o ../../file/whole_file.o ../../util/string.o

all: embed

embed: $(OBJ)
	g++ -o $@ $^

clean:
	rm -f embed $(OBJ)