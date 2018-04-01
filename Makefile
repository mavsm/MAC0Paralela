CFLAGS = -lpthread -g
CC = gcc

default:
	${CC}  desafio2.c ${CFLAGS} -o executavel

run4:
	./executavel 10000000 4 < entrada

run2:
	./executavel 10000000 4 < entrada2