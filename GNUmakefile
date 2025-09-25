all: neblina

#
# objects
# 

ifeq ($(OS),Windows_NT)
  OS=win32
else
  OS=posix
endif
include objects.mk

#
# flags
#

INCLUDES=-I. -Isrc -Isrc/util -Isrc/contrib/json-gen-c/src -isystem src/contrib/miniz -isystem src/contrib -D__STDC_WANT_LIB_EXT2__=1 -D_POSIX_C_SOURCE=200809L

CFLAGS=-std=c17
CPPFLAGS=-MMD $(INCLUDES)

ifdef DEV
  CPPFLAGS += -O0 -ggdb -fno-inline-functions -fstack-protector-strong -fno-common -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wno-strict-prototypes -Wno-newline-eof
  ifeq ($(CC),g++)
    CPPFLAGS += -fanalyzer -Wlogical-op -Wduplicated-cond -Wduplicated-branches
  endif
else
  CPPFLAGS += -Os -ffast-math -march=native -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fno-common
  LDFLAGS += -s -flto=auto
endif

CPPFLAGS_CONTRIB = -I. -O3 -ffast-math -march=native -flto

UNAME_S = $(shell uname -s)

#
# auto-generated files
#

# embedded files

init.gen.inc: embed
	./embed -d src/init > $@

src/main.o: init.gen.inc

# config files

$(OBJ): src/config/config.gen.o

src/config/config.gen.c: src/config/config.json-gen-c json-gen-c
	./json-gen-c -in $< -out src/config/
	mv src/config/json.gen.c src/config/config.gen.c
	mv src/config/json.gen.h src/config/config.gen.h
	sed -i -e 's/JSON_GEN/CONFIG_GEN/g' src/config/config.gen.h
	sed -i -e 's/json.gen.h/config.gen.h/g' src/config/config.gen.c
	rm -f src/config/sstr.*

INTERMEDIATE = init.gen.inc src/config/config.gen.c src/config/config.gen.h

#
# targets
# 

src/contrib/miniz/miniz.o: src/contrib/miniz/miniz.c
	$(CC) -c $(CPPFLAGS_CONTRIB) -o $@ $^     # compile without warnings or deep checks

neblina: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
ifeq ($(UNAME_S),Linux)
	sudo setcap cap_net_bind_service=ep ./$@
endif

embed: CPPFLAGS = $(INCLUDES) -Wextra -ggdb -O0
embed: tools/embed/embed.o src/file/whole_file.o src/main/error.o src/util/logs.o src/file/gz.o src/main/args.o src/os/posix/window.o src/contrib/miniz/miniz.o
	$(CC) -o $@ $^

test: neblina
	cd ../tests && python3 -m unittest

leaks: all
ifeq ($(UNAME_S),Linux)
	$(if $(SERVICE),,$(error Variable SERVICE must be defined))
	sudo setcap -r ./neblina || true
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind.supp ./neblina -s $(SERVICE) -v
else
	$(error Checking for leaks only supported on Linux.)
endif

json-gen-c:
	cd src/contrib/json-gen-c && $(MAKE)
	cp src/contrib/json-gen-c/build/bin/json-gen-c .

dev:
	$(MAKE) all DEV=1

bear:
	make clean
	bear -- make

clean:
	find . -type f -name '*.[od]' -exec rm {} +
	cd src/contrib/json-gen-c && $(MAKE) -s clean > /dev/null
	rm -f neblina embed json-gen-c $(INTERMEDIATE)

install: neblina
	install neblina /usr/local/bin/neblina

uninstall:
	rm /usr/local/bin/neblina

-include $(OBJ:.o=.d)

# vim:sts=8:ts=8:sw=8:noexpandtab
