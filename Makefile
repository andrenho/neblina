# This file is meant to be used by 'nmake' on Windows. See GNUmakefile for other POSIX OSes.

all: neblina.exe

# 
# Objects
#

OS=win32
!INCLUDE objects.mk
OBJ = $(OBJ:.o=.obj)

#
# Flags
#
INCLUDES=/I. /Isrc /I"src\contrib\miniz" /I"src\contrib" /I"src\util" /Dfallthrough
CFLAGS = /std:c17 /DSTDC_WANT_LIB_EXT2__=1
CPPFLAGS = /nologo /MD /D_CRT_SECURE_NO_WARNINGS $(INCLUDES)

!IFDEF DEV
CPPFLAGS = $(CPPFLAGS) /Od /Zi /GS /W4
!ELSE
CPPFLAGS = $(CPPFLAGS) /O2 /GL /GS /D_FORTIFY_SOURCE=2
LDFLAGS  = /LTCG /INCREMENTAL:NO /OPT:REF /OPT:ICF
!ENDIF

CPPFLAGS_CONTRIB = /nologo /MD /I. /O2 /GL

#
# auto-generated files
#

init.gen.inc: embed.exe
	embed -d src\init > $@

src\main.obj: init.gen.inc

INTERMEDIATE = init.gen.inc


#
# Targets
#

.SUFFIXES: .c .obj


src\contrib\miniz\miniz.obj: src\contrib\miniz\miniz.c
	cl /c $(CPPFLAGS_CONTRIB) /Fo$@ $**

.c.obj:
	cl /c $(CPPFLAGS) /Fo$@ $<

neblina.exe: $(OBJ)
	link /nologo $(LDFLAGS) /OUT:$@ $(OBJ)

embed.exe: tools\embed\embed.obj src\file\whole_file.obj src\file\gz.obj src\contrib\miniz\miniz.obj src\main\error.obj src\util\logs.obj src\file\gz.obj src\main\args.obj src\os\posix\window.obj
	link /nologo /OUT:$@ $**

test: neblina.exe
	python -m unittest discover ..\tests

dev:
	nmake all DEV=1

clean:
	del neblina.exe $(INTERMEDIATE)
	cmd /V:ON /C "set P=$(OBJ) & del !P:/=\!"

