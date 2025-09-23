all: neblina

#
# objects
# 

OBJ = src/main.o \
      src/main/args.o src/main/error.o \
      src/file/whole_file.o

#
# flags
#

CFLAGS=-std=c23
CPPFLAGS=-MMD -I. -isystem contrib/miniz

ifdef DEV
  CPPFLAGS += -O0 -ggdb -fno-inline-functions -fstack-protector-strong -fno-common -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align
  ifeq ($(CC),g++)
    CPPFLAGS += -fanalyzer -Wlogical-op -Wduplicated-cond -Wduplicated-branches
  endif
else
  CPPFLAGS += -Os -ffast-math -march=native -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fno-common
  LDFLAGS += -flto=auto -s
endif

# 
# targets
# 

contrib/simdjson/miniz.o: contrib/miniz/miniz.c
	$(CC) -c -I. -O3 -ffast-math -march=native -flto -o $@ $^     # compile without warnings or deep checks

neblina: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	sudo setcap cap_net_bind_service=ep ./$@

test: neblina
	cd ../tests && python3 -m unittest

leaks: all
	$(if $(SERVICE),,$(error Variable SERVICE must be defined))
	sudo setcap -r ./neblina || true
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind.supp ./neblina -s $(SERVICE) -v

dev:
	$(MAKE) all DEV=1

clean:
	rm -f $(OBJ) $(OBJ:.o=.d) neblina

-include $(OBJ:.o=.d)

# vim:sts=8:ts=8:sw=8:noexpandtab
