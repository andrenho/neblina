#
# objects
#

OBJ=	main.o				arguments.o				\
	orchestrator/orchestrator.o	orchestrator/orchestrator_config.o	\
	service/scheduled_service.o						\
	util/embed/embedded.o							\
	contrib/simdjson/simdjson.o

#
# config
#

JSON2CPP = ../tools/json2cpp/json2cpp.py

#
# flags
#

CPPFLAGS=-Wall -MMD -I.
CXXFLAGS=-std=c++23
LDFLAGS=-lz -flto=auto

ifdef DEV
  CPPFLAGS += -O0 -ggdb
else ifdef PEDANTIC
  CPPFLAGS += -O0 -ggdb -fstack-protector-strong -fno-common -Wextra -Wpedantic -Wshadow -Wformat=2
  CXXFLAGS += -Weffc++ -Wnull-dereference -Wcast-align
  ifeq ($(CC),g++)
    CXXFLAGS += -fanalyzer -Wlogical-op -Wduplicated-cond -Wduplicated-branches
  endif
else
  CPPFLAGS += -O3 -ffast-math -march=native -flto -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fno-common
endif

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
  STAT_PRM="-f %z"
else
  STAT_PRM="-c %s"
endif

#
# rules
#

all: neblina

watchdog/watchdog.o: watchdog/watchdog.gen.inc

contrib/simdjson/simdjson.o: contrib/simdjson/simdjson.cpp
	g++ -c -I. -O3 -ffast-math -march=native -flto -o $@ $^     # compile without warnings or deep checks

%.gen.inc: %.json
	echo "#include <stdint.h>" > $@ && \
	echo "#include <stddef.h>" >> $@ && \
	echo "static const uint8_t $(basename $(notdir $^))[] = {" >> $@ && \
	cat $^ | gzip | xxd -i >> $@ && \
	echo "};" >> $@
	echo "static size_t $(basename $(notdir $^))_uncompressed_sz = $(shell stat ${STAT_PRM} $^);" >> $@

orchestrator/orchestrator.o: orchestrator/orchestrator_config.o

orchestrator/orchestrator_config.o: orchestrator/orchestrator_config.cc

orchestrator/orchestrator_config.cc: orchestrator/orchestratorConfigSchema.json
	$(JSON2CPP) $^ OrchestratorConfig

pedantic:
	$(MAKE) all PEDANTIC=1

dev:
	$(MAKE) all DEV=1

neblina: $(OBJ)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(OBJ) $(OBJ:.o=.d) neblina **/*.gen.inc *.gen.inc

-include $(OBJ:.o=.d)

# vim:sts=8:ts=8:sw=8:expandtab
