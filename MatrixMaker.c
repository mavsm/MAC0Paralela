#include <stdio.h>
#include <stdlib.h>

//faz uma matriz 15x100 e uma 100x49

int main() {
	FILE *AFile, *BFile;
	int i, j;

	AFile = fopen("testA.txt", "w");
	BFile = fopen("testB.txt", "w");

	fprintf(AFile, "1000 20000\n");
	fprintf(BFile, "20000 999\n");

	for(i=0; i<1000; i++) {
		for(j=0; j<20000; j++) {
			if((float)rand()/RAND_MAX > .5) {
				fprintf(AFile, "%d %d %lf\n", i, j, (double)rand()/RAND_MAX);
			}
		}
	}
	for(i=0; i<20000; i++) {
		for(j=0; j<999; j++) {
			if((float)rand()/RAND_MAX > .5) {
				fprintf(BFile, "%d %d %lf\n", i, j, (double)rand()/RAND_MAX);
			}
		}
	}

	fclose(AFile);
	fclose(BFile);

	return 0;
}