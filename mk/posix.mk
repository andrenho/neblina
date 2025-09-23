CFLAGS=-std=c23
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
RM=rm -f
