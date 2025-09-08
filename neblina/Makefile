CPPFLAGS=-Wall -Wextra -MMD
CXXFLAGS=-std=c++23
OBJ=main.o

ifdef RELEASE
	CPPFLAGS+=-Ofast -march=native -flto
else
	CPPFLAGS+=-O0 -ggdb
endif

all: neblina

release: RELEASE=1
release: all

neblina: $(OBJ)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(OBJ:.o=.d) neblina

-include $(OBJ:.o=.d)
