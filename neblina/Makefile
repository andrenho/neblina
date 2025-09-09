CPPFLAGS=-Wall -Wextra -MMD -I.
CXXFLAGS=-std=c++23
LDFLAGS=-lz
OBJ=main.o \
	util/config/config_manager.o    \
	util/embed/embedded.o 		\
	util/tcp/tcp_server.o 		\
	watchdog/watchdog.o 		\
	watchdog/watchdog_config.o

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

util/config/config_manager.o: default_config.gen.inc

%.gen.inc: %.toml
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
