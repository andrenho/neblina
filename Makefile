
# 
# Objects
#

!INCLUDE objects.mk
OBJ = $(OBJ:.o=.obj)

#
# Flags
#
CFLAGS = /std:c17 /DSTDC_WANT_LIB_EXT2__=1
CPPFLAGS = /nologo /MD /D_CRT_SECURE_NO_WARNINGS /I. /Isrc /I"src\contrib\miniz"

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



#
# Targets
#

.SUFFIXES: .c .obj

all: neblina.exe

src\contrib\miniz\miniz.obj: src\contrib\miniz\miniz.c
	cl /c $(CPPFLAGS_CONTRIB) /Fo$@ $**

.c.obj:
	cl /c $(CPPFLAGS) /Fo$@ $<

neblina.exe: $(OBJ)
	link /nologo $(LDFLAGS) /OUT:$@ $(OBJ)

embed.exe: tools\embed\embed.obj src\file\whole_file.obj src\file\gz.obj src\contrib\miniz\miniz.obj src\main\err.obj
	link /nologo /OUT:$@ $**

test: neblina.exe
	python -m unittest discover ..\tests

dev:
	nmake all DEV=1

clean:
	-del /Q $(OBJ) neblina.exe

