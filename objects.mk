OBJ = src/main.o \
      src/util/logs.o \
      src/os/$(OS)/os.o src/os/$(OS)/fs.o src/os/$(OS)/window.o \
      src/main/args.o src/main/error.o \
      src/file/whole_file.o src/file/gz.o src/file/fileset.o \
      src/contrib/miniz/miniz.o
