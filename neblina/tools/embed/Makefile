CPPFLAGS=-Wall -Wextra -g -O0 -I../../contrib/miniz

all: embed

embed: main.o ../../contrib/miniz/miniz.o ../../file/gz.o
	g++ -o $@ $^

clean:
	rm -f embed main.o ../../contrib/miniz/miniz.o