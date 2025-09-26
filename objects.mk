OBJ = src/main.o \
      src/config/config.o src/config/args.o \
      src/contrib/miniz/miniz.o src/contrib/microjson/mjson.o \
      src/file/whole_file.o src/file/gz.o src/file/fileset.o \
      src/os/$(OS)/os.o src/os/$(OS)/fs.o src/os/$(OS)/window.o \
      src/server/tcp_server.o src/server/poller/poller_$(OS_SPECIFIC).o \
      src/service/orchestrator.o \
      src/services/parrot/parrot.o \
      src/util/logs.o src/util/error.o
