# MinGW 5.1.6
.PHONY:clean tidy rebuild build

SHELL   =   cmd

SRCDIR      =   src/main
HEADDIR     =   src/head
LIBDIR      =   lib
DOCSDIR     =   doc
OUTDIR      =   out

CC      =   g++
SRCEXT  =   cpp
DEPEXT  =   depend
RM      =   del /f /q
MKDIR   =   mkdir
RMDIR   =   rmdir /q /s
FLAGS       =   -g -Wall -O3 -finput-charset=UTF-8 -fexec-charset=GB2312
CFLAGS      =   -I"${HEADDIR}"
LIBPATHS    =   -L"C:\MingW\lib" -L"${LIBDIR}"
LIBS        =   #-lopengl32 #-mwindows
SRCT        =   *.${SRCEXT}
SRCS        +=  ${SRCT}
SRCS        +=  */${SRCT}
SRCS        +=  */*/${SRCT}
SRCS        +=  */*/*/${SRCT}
SRCS        +=  */*/*/*/${SRCT}
SRCS        +=  */*/*/*/*/${SRCT}
SRC     =   ${wildcard ${addprefix ${SRCDIR}/,${SRCS}}}
OBJ     =   ${patsubst ${SRCDIR}/%.${SRCEXT},${OUTDIR}/%.o,${SRC}}
DEP     =   ${patsubst ${SRCDIR}/%.${SRCEXT},${OUTDIR}/%.${DEPEXT},${SRC}}
BIN     =   ${OUTDIR}/${notdir ${CURDIR}.exe}

build: ${BIN}
	@echo start ${BIN} > run.bat
	@run.bat
	@${RM} run.bat
rebuild: tidy build
${BIN}: ${OBJ}
	@echo make target: ${BIN}
	@${CC} ${FLAGS} ${CFLAGS} $^ ${LIBPATHS} ${LIBS} -o $@
prepare:
	@echo prepare folders
	@if not exist ${OUTDIR} (${MKDIR} ${OUTDIR} && @echo make directory: ${OUTDIR})
	@if not exist ${DOCSDIR} (${MKDIR} ${DOCSDIR} && @echo make directory: ${DOCSDIR})
#	@if not exist ${subst /,\,${dir ${@}}} (${MKDIR} ${subst /,\,${dir ${@}}} && @echo make directory: ${subst /,\,${dir ${@}}})
	@xcopy ${SRCDIR} ${OUTDIR} /T /E
clean:
	@echo remove build directory and docs directory
	@if exist ${OUTDIR} (@${RMDIR} ${OUTDIR})
	@if exist ${DOCSDIR} (@${RMDIR} ${DOCSDIR})
tidy:
	@echo tidy dependencies, objects and binaries only
	${foreach file,${subst /,\,${OBJ}},@if exist ${file} (${RM} ${file}) &&}@echo on
	${foreach file,${subst /,\,${DEP}},@if exist ${file} (${RM} ${file}) &&}@echo on
sinclude ${DEP}
${OUTDIR}/%.${DEPEXT}:${SRCDIR}/%.${SRCEXT}
	@if not exist ${OUTDIR} (${MKDIR} ${OUTDIR} && @echo make directory: ${OUTDIR})
	@if not exist ${subst /,\,${dir ${@}}} (${MKDIR} ${subst /,\,${dir ${@}}} && @echo make directory: ${subst /,\,${dir ${@}}})
	@echo make dependency for ${patsubst ${SRCDIR}/%.${SRCEXT},${OUTDIR}/%.o,${<}}
	@${CC} ${CFLAGS} -MM -MT ${patsubst ${SRCDIR}/%.${SRCEXT},${OUTDIR}/%.o,${<}} ${<} > ${@}
	@echo 	@@echo make object: ${patsubst ${SRCDIR}/%.${SRCEXT},${OUTDIR}/%.o,${<}} >> ${@}
	@echo 	@@$${CC} $${FLAGS}  $${CFLAGS} -c -o $$@ $${filter %%.${SRCEXT},$$^^} >> ${@}