CFLAGS = -lpthread -Wall -g
CC = gcc

default:
	${CC}  matMul.c ${CFLAGS} -o executavel -fopenmp

pexe:
	./executavel p testA.txt testB.txt testC.txt

oexe:
	./executavel o testA.txt testB.txt testC.txt
