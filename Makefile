CFLAGS = -lpthread -Wall -g
CC = gcc

default:
	nvcc  reduction.cu ${CFLAGS} -o executavel

