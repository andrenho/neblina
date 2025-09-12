CPPFLAGS=-Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -MMD -I. -fstack-protector-strong -fno-common -D_FORTIFY_SOURCE=2
CXXFLAGS=-std=c++23 -Weffc++ -Wnull-dereference -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wcast-align -fanalyzer
LDFLAGS=-lz
OBJ=main.o				\
    contrib/simdjson/simdjson.o		\

ifdef RELEASE
  CPPFLAGS+=-O3 -ffast-math -march=native -flto
else
  CPPFLAGS+=-O0 -ggdb
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  STAT_PRM="-f %z"
else
  STAT_PRM="-c %s"
endif

all: neblina

watchdog/watchdog.o: watchdog/watchdog.gen.inc

%.gen.inc: %.json
	echo "#include <stdint.h>" > $@ && \
	echo "#include <stddef.h>" >> $@ && \
	echo "static const uint8_t $(basename $(notdir $^))[] = {" >> $@ && \
	cat $^ | gzip | xxd -i >> $@ && \
	echo "};" >> $@
	echo "static size_t $(basename $(notdir $^))_uncompressed_sz = $(shell stat ${STAT_PRM} $^);" >> $@

release:
	$(MAKE) clean all RELEASE=1

neblina: $(OBJ)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(OBJ:.o=.d) neblina **/*.gen.inc *.gen.inc

-include $(OBJ:.o=.d)
