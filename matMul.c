#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

double **A, **B, **C;

//lembremos que C[a][b] = SUM(A[a][0..P]B[0..P][b])
double multiplicaLocal(int a, int b, int P){
	double sum = 0;
	int i;
	for(i=0; i<P; i++){
		sum += A[a][i]*B[i][b];
	}
	return sum;
}


int main(int argc, char **argv) {
	FILE *AFile, *BFile, *CFile;
	int i, N, M, P;

	if(argc != 4) printf("Número errado de argumentos, devem ser 3.\n");

	AFile = fopen(argv[1], "r"); 
	BFile = fopen(argv[2], "r");
	CFile = fopen(argv[3], "w");

	fscanf(AFile, "%d %d", &N, &P);
	A = malloc(N*sizeof(double*));
	for(i=0; i<N; i++) A[i] = malloc(P*sizeof(double));

	fscanf(BFile, "%d %d", &P, &M);
	B = malloc(P*sizeof(double*));
	for(i=0; i<P; i++) B[i] = malloc(M*sizeof(double));

	C = malloc(P*sizeof(double*));
	for(i=0; i<P; i++) C[i] = malloc(P*sizeof(double));




	fclose(AFile);
	fclose(BFile);
	fclose(CFile);

	for(i=0; i<N; i++) free(A[i]);
	free(A);

	for(i=0; i<P; i++) free(B[i]);
	free(B);

	for(i=0; i<P; i++) free(C[i]);
	free(C);
	

}