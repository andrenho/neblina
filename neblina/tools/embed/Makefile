CPPFLAGS=-Wall -Wextra -g -O0 -I../../contrib/miniz

all: embed

embed: main.o ../../contrib/miniz/miniz.o
	gcc -o $@ $^

clear:
	rm -f embed main.o ../../contrib/miniz/miniz.o