CFLAGS = -lpthread -Wall -g
CC = gcc

default:
	${CC}  matMul.c ${CFLAGS} -o executavel -fopenmp

