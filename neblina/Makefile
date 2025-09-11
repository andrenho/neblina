CPPFLAGS=-Wall -Wextra -MMD -I.
CXXFLAGS=-std=c++23
LDFLAGS=-lz
OBJ=main.o arguments.o \
	util/embed/embedded.o 		\
	service/config.o		\
	service/tcp/tcp_server.o	\
	service/tcp/tcp_server_text.o	\
	http11/http_11.o		\
	http11/http_11_config.o		\
	watchdog/watchdog.o 		\
	watchdog/watchdog_config.o	\
	contrib/simdjson/simdjson.o

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
