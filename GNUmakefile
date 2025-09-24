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

INCLUDES=-I. -Isrc -isystem src/contrib/miniz -isystem src/contrib/jsmn -D__STDC_WANT_LIB_EXT2__=1 -D_POSIX_C_SOURCE=200809L

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

init.gen.inc: embed
	./embed -d src/init > $@

src/main.o: init.gen.inc

INTERMEDIATE = init.gen.inc

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

dev:
	$(MAKE) all DEV=1

bear:
	make clean
	bear -- make

clean:
	find . -type f -name '*.[od]' -exec rm {} +
	rm -f neblina embed $(INTERMEDIATE)

install: neblina
	install neblina /usr/local/bin/neblina

uninstall:
	rm /usr/local/bin/neblina

-include $(OBJ:.o=.d)

# vim:sts=8:ts=8:sw=8:noexpandtab
