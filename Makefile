CFLAGS = -lpthread
# -ansi -Wall -O2 -g
CC = gcc

default:
	${CC}  miniep2.c ${CFLAGS} -o executavel

compile:
	${CC}  miniep2.c ${CFLAGS} -o executavel

ex:
	./executavel 5