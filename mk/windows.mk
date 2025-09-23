CFLAGS=-std:c23
CPPFLAGS=-I. -Isrc -Isrc/contrib/miniz

ifdef DEV
  # Debug build: disable optimization, add debug info, runtime checks, strong stack checks
  CPPFLAGS += -Od -Z7 -RTC1 -GS -W4
  # If compiling C++ code, enable analyzer
  ifeq ($(CC),cl)
    CPPFLAGS += -analyze
  endif
else
  # Release build: optimize, enable LTO (/GL), target native arch
  CPPFLAGS += -O2 -GL -arch:AVX2 -GS -D_FORTIFY_SOURCE=2
  LDFLAGS += -LTCG -OPT:REF -OPT:ICF
endif

# Contrib: build with optimizations but without warnings/deep checks
CPPFLAGS_CONTRIB = -I. -O2 -GL -arch:AVX2

RM=del /Q

