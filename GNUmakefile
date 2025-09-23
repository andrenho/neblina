all: neblina

#
# objects
# 

OBJ = src/main.o \
      src/main/args.o src/main/error.o \
      src/file/whole_file.o \
      src/contrib/miniz/miniz.o

#
# flags
# 

CFLAGS=-std=c99 -D__STDC_WANT_LIB_EXT2__=1
CPPFLAGS=-MMD -I. -Isrc -isystem src/contrib/miniz

ifdef DEV
  CPPFLAGS += -O0 -ggdb -fno-inline-functions -fstack-protector-strong -fno-common -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align
  ifeq ($(CC),g++)
    CPPFLAGS += -fanalyzer -Wlogical-op -Wduplicated-cond -Wduplicated-branches
  endif
else
  CPPFLAGS += -Os -ffast-math -march=native -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fno-common
  LDFLAGS += -flto=auto -s
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
	rm -f $(OBJ) $(OBJ:.o=.d) neblina

-include $(OBJ:.o=.d)

# vim:sts=8:ts=8:sw=8:noexpandtab
