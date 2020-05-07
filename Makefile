
##### DIRECTORIES

INC  = include
SRC  = source
BLD  = build





##### LIBRARIES

HDR  = ${INC}/petri.h

LIB  = -L/usr/lib
LIB += -lSDL2
LIB += -lSDL2_image
LIB += -lpthread





##### COMPILER OPTIONS

CFLG = -Wall -std=c++17 -g
CINC = -I${INC}
COPT = ${CFLG} ${CINC}
CC   = g++-8 ${COPT}





##### BUILD OBJECTS 

OBJ  = ${BLD}/main.o
OBJ += ${BLD}/constants.o
OBJ += ${BLD}/App.o
OBJ += ${BLD}/Dish.o

EXE  = petri





##### RECIPES

all: ${BLD} ${OBJ} ${EXE}

${BLD}:
	mkdir -p ${BLD}

clean:
	rm -f ${OBJ}
	rm -f ${EXE}
	rmdir ${BLD}

rebuild: clean all





##### App OBJECT FILES

${BLD}/App.o: ${SRC}/App.cpp ${HDR}
	${CC} -o $@ -c $<

${BLD}/Dish.o: ${SRC}/Dish.cpp ${HDR}
	${CC} -o $@ -c $<




##### MISC OBJECT FILES

${BLD}/main.o: ${SRC}/main.cpp ${HDR}
	${CC} -o $@ -c $<

${BLD}/constants.o: ${SRC}/constants.cpp ${HDR}
	${CC} -o $@ -c $<




##### EXECUTABLE

${EXE}: ${OBJ}
	${CC} -o $@ $^ ${LIB}






