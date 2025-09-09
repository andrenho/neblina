CPPFLAGS=-Wall -Wextra -MMD -I.
CXXFLAGS=-std=c++23
LDFLAGS=-lz
TXT_OBJ=main/default_config.gen.o
OBJ=$(TXT_OBJ) main/main.o main/config.o main/embedded.o

ifdef RELEASE
	CPPFLAGS+=-O3 -ffast-math -march=native -flto
else
	CPPFLAGS+=-O0 -ggdb
endif

all: neblina

%.gen.c: %.toml
	echo "#include <stdint.h>" > $@ && \
	echo "#include <stddef.h>" >> $@ && \
	echo "static const uint8_t $(basename $(notdir $^))[] = {" >> $@ && \
	cat $^ | gzip | xxd -i >> $@ && \
	echo "};" >> $@
	echo "static size_t $(basename $(notdir $^))_uncompressed_sz = $(shell stat -c %s $^);" >> $@

release:
	$(MAKE) clean all RELEASE=1

neblina: $(OBJ)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(OBJ:.o=.d) $(TXT_OBJ:.gen.o=.gen.c) neblina

-include $(OBJ:.o=.d)
