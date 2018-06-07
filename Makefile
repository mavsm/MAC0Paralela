CFLAGS = -lpthread
CC = nvcc

default:
	${CC}  reduction.cu ${CFLAGS} -o executavel

