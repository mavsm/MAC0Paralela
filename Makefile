CFLAGS = -lpthread -Wall -g
CC = gcc

default:
	${CC}  miniep2.c ${CFLAGS} -o executavel

run:
	./executavel 5