CFLAGS = -lpthread -g -G
CC = nvcc

default:
	${CC}  reduction.cu ${CFLAGS} -o executavel

run:
	./executavel test.txt

createTest:
	gcc matrixMaker.c -o matrixMaker
	./matrixMaker
