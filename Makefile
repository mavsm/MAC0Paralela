CFLAGS = -lpthread
CC = nvcc

default:
	${CC}  reduction.cu ${CFLAGS} -o executavel

makeTest:
	gcc matrixMaker.c -o matrixMaker
	./matrixMaker
