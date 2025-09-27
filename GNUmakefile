all: neblina

#
# objects
#

ifeq ($(OS),Windows_NT)
  OS=win32
  OS_SPECIFIC=win32
else
  OS=posix
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    OS_SPECIFIC=apple
  else
    OS_SPECIFIC=unix
  endif
endif

THREAD = multi
ifdef SINGLE_THREAD
  THREAD = single
endif

include objects.mk

#
# flags
#

INCLUDES=-I. -Isrc -Isrc/util -isystem src/contrib/miniz -isystem src/contrib -D__STDC_WANT_LIB_EXT2__=1 -D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE

CFLAGS=-std=c17
CPPFLAGS=-MMD $(INCLUDES)

ifdef DEV
  CPPFLAGS += -O0 -ggdb -fno-inline-functions -fstack-protector-strong -fno-common -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wno-strict-prototypes -Wno-newline-eof
  ifeq ($(CC),g++)
    CPPFLAGS += -fanalyzer -Wlogical-op -Wduplicated-cond -Wduplicated-branches
  endif
else
  CPPFLAGS += -Os -ffast-math -march=native -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fno-common
  LDFLAGS += -flto=auto
endif

CPPFLAGS_CONTRIB = -I. -O3 -ffast-math -march=native -flto -Wno-switch

ifdef SERVICE
  SERVICE_OPT := -s $(SERVICE)
endif

#
# auto-generated files
#

# embedded files

init.gen.inc: embed
	./embed -d src/init > $@

src/main.o: init.gen.inc

INTERMEDIATE = init.gen.inc

#
# targets
# 

src/contrib/miniz/miniz.o: src/contrib/miniz/miniz.c
	$(CC) -c $(CPPFLAGS_CONTRIB) -o $@ $^     # compile without warnings or deep checks

src/contrib/microjson/mjson.o: src/contrib/microjson/mjson.c
	$(CC) -c $(CPPFLAGS_CONTRIB) -o $@ $^     # compile without warnings or deep checks

neblina: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
ifndef DEV
	strip ./$@
endif
ifeq ($(UNAME_S),Linux)
	sudo setcap cap_net_bind_service=ep ./$@
endif

embed: CPPFLAGS = $(INCLUDES) -Wextra -ggdb -O0
embed: tools/embed/embed.o src/file/whole_file.o src/util/error.o src/util/logs.o src/file/gz.o src/config/args.o src/os/posix/window.o src/contrib/miniz/miniz.o
	$(CC) -o $@ $^

test: neblina
	cd ../tests && python3 -m unittest

leaks: all
ifeq ($(UNAME_S),Linux)
	$(info Use SERVICE variable to start a service)
	sudo setcap -r ./neblina || true
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind.supp ./neblina $(SERVICE_OPT) -P 5000 -v
else
	$(error Checking for leaks only supported on Linux.)
endif

thread-check: all
ifeq ($(UNAME_S),Linux)
	$(info Use SERVICE variable to start a service)
	sudo setcap -r ./neblina || true
	valgrind --tool=drd ./neblina $(SERVICE_OPT) -P 5000 -v
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
