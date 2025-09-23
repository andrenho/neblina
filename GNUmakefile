all: neblina

#
# objects
# 

include objects.mk

#
# flags
# 

CFLAGS=-std=c17 -D__STDC_WANT_LIB_EXT2__=1
CPPFLAGS=-MMD -I. -Isrc -isystem src/contrib/miniz
LDFLAGS=-flto=auto

ifdef DEV
  CPPFLAGS += -O0 -ggdb -fno-inline-functions -fstack-protector-strong -fno-common -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wno-strict-prototypes
  ifeq ($(CC),g++)
    CPPFLAGS += -fanalyzer -Wlogical-op -Wduplicated-cond -Wduplicated-branches
  endif
else
  CPPFLAGS += -Os -ffast-math -march=native -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fno-common
  LDFLAGS += -s
endif

CPPFLAGS_CONTRIB = -I. -O3 -ffast-math -march=native -flto

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

embed: CPPFLAGS = -I. -Isrc -isystem src/contrib/miniz -Wall -Wextra -ggdb -O0 -D_GNU_SOURCE=1
embed: tools/embed/embed.o src/file/whole_file.o src/file/gz.o src/contrib/miniz/miniz.o
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
	rm -f neblina embed


-include $(OBJ:.o=.d)

# vim:sts=8:ts=8:sw=8:noexpandtab
