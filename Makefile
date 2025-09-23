all: neblina

#
# OS-specific
#

ifeq ($(OS),Windows_NT)
  include mk/windows.mk
  UNAME_S := Windows
else
  include mk/posix.mk
  UNAME_S := $(shell uname -s)
endif

#
# objects
# 

OBJ = src/main.o \
      src/main/args.o src/main/error.o \
      src/file/whole_file.o \
      src/contrib/miniz/miniz.o

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
	$(RM) $(OBJ) $(OBJ:.o=.d) neblina

-include $(OBJ:.o=.d)

# vim:sts=8:ts=8:sw=8:noexpandtab
