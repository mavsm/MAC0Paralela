#include <stdio.h>
#include <stdlib.h>

//faz uma matriz 15x100 e uma 100x49

int main() {
	FILE *file;
	int numMatrix = 100;
	int cont, i, j;

	file = fopen("test.txt", "w");

	fprintf(file, "%d\n***\n", numMatrix);

	for(cont=0; cont<numMatrix; cont++) {
		for(i=0; i<3; i++) {
			fprintf(file, "%d %d %d\n", rand()%100, rand()%100, rand()%100);
		}
		fprintf(file, "***\n");
	}
	fclose(file);

	return 0;
}