CPPFLAGS=-Wall -Wextra -MMD
CXXFLAGS=-std=c++23
OBJ=main/main.o main/config.o

ifdef RELEASE
	CPPFLAGS+=-O3 -ffast-math -march=native -flto
else
	CPPFLAGS+=-O0 -ggdb
endif

all: neblina

%.o: %.toml
	# objcopy -I binary -O

release:
	$(MAKE) clean all RELEASE=1

neblina: $(OBJ)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(OBJ:.o=.d) neblina

-include $(OBJ:.o=.d)
