#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

double **A, **B, **C;
int M, N, P;

//lembremos que C[a][b] = SUM(A[a][0..P]B[0..P][b])
double multiplicaLocal(int a, int b){
	double sum = 0;
	int i;
	for(i=0; i<P; i++){
		sum += A[a][i]*B[i][b];
	}
	return sum;
}

//Função de cada thread, representa uma coluna de C
void *func_threads(void *arg) {
	int id = *(int*)arg;
	int i;

	for(i=0; i<M; i++) {
		C[i][id] = multiplicaLocal(i, id)

	}
	pthread_exit(NULL);
}

//Funcão que coordena as threads
void MatMul_ptrheads() {
	pthread_t *threads;
	int i, j;

	threads = malloc(N*sizeof(pthread_t));

	//A minha ideia é fazer t <= N threads, e fazer cada coluna/linha de C paralelamente
	for(i=0; i<N; i++)
		pthread_create(&threads[i], NULL, func_threads, &(i));

	for(i=0, i<N; i++)
		pthread_join(threads[i], NULL);
}

//Função de coordenação de omp
void MatMul_omp() {}


int main(int argc, char **argv) {
	FILE *AFile, *BFile, *CFile;
	int i, j;
	double v;

	if(argc != 5) printf("Número errado de argumentos, devem ser 4.\n");

	AFile = fopen(argv[2], "r"); 
	BFile = fopen(argv[3], "r");
	CFile = fopen(argv[4], "w");

	//Inicializa as matrizes:

	fscanf(AFile, "%d %d", &N, &P);
	A = malloc(N*sizeof(double*));
	for(i=0; i<N; i++) A[i] = malloc(P*sizeof(double));

	fscanf(BFile, "%d %d", &P, &M);
	B = malloc(P*sizeof(double*));
	for(i=0; i<P; i++) B[i] = malloc(M*sizeof(double));

	C = malloc(P*sizeof(double*));
	for(i=0; i<P; i++) C[i] = malloc(P*sizeof(double));

	//Le as matrizes:
	for(i=0; i<N; i++)
		for(j=0; j<P; j++)
			A[i][j] = 0;
	for(i=0; i<P; i++)
		for(j=0; j<M; j++)
			B[i][j] = 0;

	while(fscanf(AFile, "%d %d %lf", &i, &j, &v))
		A[i][j] = v;
	while(fscanf(BFile, "%d %d %lf", &i, &j, &v))
		B[i][j] = v;
	
	
	//Implementação
	if(argv[1] == "p") //implementação em ptrheads
		MatMul_ptrheads();
	if(argv[1] == "o") //implementação em omp
		MatMul_omp();


	//Fechamento
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