#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <omp.h>

double **A, **B, **C;

int M, N, P;

int NUM_THREADS = 10;

pthread_mutex_t idMutex;

//A[M][P]
//B[P][N]
//C[M][N]


//LEMBRANDO QUE O A USADO EH TAL QUE B(usado) = B(dado)^T


//lembremos que C[a][b] = SUM(A[a][0..P]B[0..P][b])
double multiplicaLocal(int a, int b){
	double sum = 0;
	int i;
	for(i=0; i<P; i++){
		sum += A[a][i]*B[b][i];
	}
	return sum;
}

//Função de cada thread, representa uma coluna de C
void *func_threads(void *arg) {
	int id = *(int*)arg;
	int i, turno;
	pthread_mutex_unlock(&idMutex);

	for(turno = 0; id + turno*NUM_THREADS<M; turno++){ 
		for(i=0; i<N; i++) {
			C[id + turno*NUM_THREADS][i] = multiplicaLocal(id, i);
		}
	}
	pthread_exit(NULL);
}

//Funcão que coordena as threads
void MatMul_ptrheads() {
	pthread_t *threads;
	int i, id;

	if(M < NUM_THREADS) NUM_THREADS = M;

	threads = malloc(NUM_THREADS*sizeof(pthread_t));

	//A minha ideia é fazer t <= M threads, e fazer cada coluna/linha de C paralelamente
	pthread_mutex_init(&idMutex, NULL);
	for(i=0; i<NUM_THREADS; i++) {
		pthread_mutex_lock(&idMutex);
		id = i;
		pthread_create(&threads[i], NULL, func_threads, &(id));
	}

	for(i=0; i<NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
}

//Função de coordenação de omp
void MatMul_omp() {
	int i, j;
	for(i=0; i<M; i++) {
		#pragma omp parallel for shared(C) private(j) num_threads(NUM_THREADS)
		for(j=0; j<N; j++)
			C[i][j] = multiplicaLocal(i, j);
	}
}


int main(int argc, char **argv) {
	FILE *AFile, *BFile, *CFile;
	int i, j;
	double v;

	if(argc != 5) printf("Número errado de argumentos, devem ser 4.\n");

	AFile = fopen(argv[2], "r"); 
	BFile = fopen(argv[3], "r");
	CFile = fopen(argv[4], "w");

	//Inicializa as matrizes:

	fscanf(AFile, "%d %d", &M, &P);
	A = malloc(M*sizeof(double*));
	for(i=0; i<M; i++) A[i] = malloc(P*sizeof(double));

	fscanf(BFile, "%d %d", &P, &N); // Esta vai ser transposta
	B = malloc(N*sizeof(double*));
	for(i=0; i<N; i++) B[i] = malloc(P*sizeof(double));

	C = malloc(M*sizeof(double*));
	for(i=0; i<M; i++) C[i] = malloc(N*sizeof(double));

	//Le as matrizes:
	for(i=0; i<M; i++)
		for(j=0; j<P; j++)
			A[i][j] = 0;
	for(i=0; i<P; i++)
		for(j=0; j<N; j++)
			B[j][i] = 0;

	while(EOF != fscanf(AFile, "%d %d %lf", &i, &j, &v)) //trabalhamos com B^T para minimizar troca de cache
		A[i][j] = v;
	while(EOF != fscanf(BFile, "%d %d %lf", &i, &j, &v))
		B[j][i] = v;
	
	
	//Implementação
	if(!strcmp(argv[1], "p")) //implementação em ptrheads
		MatMul_ptrheads();
	if(!strcmp(argv[1], "o")) //implementação em omp
		MatMul_omp();

/*
	for(i=0; i<M; i++) {
		for (j=0; j<N; j++)
			printf("%lf ", C[i][j]);
		printf("\n");
	}
*/
	//Fechamento
	pthread_mutex_destroy(&idMutex);

	//Imprime
	fprintf(CFile, "%d %d\n", M, N);
	for(i=0; i<M; i++) {
		for(j=0; j<N; j++) {
			if(C[i][j] != 0)
				fprintf(CFile, "%d %d %lf\n", i, j, C[i][j]);
		}
	}

	fclose(AFile);
	fclose(BFile);
	fclose(CFile);

	for(i=0; i<M; i++) free(A[i]);
	free(A);

	for(i=0; i<N; i++) free(B[i]);
	free(B);

	for(i=0; i<M; i++) free(C[i]);
	free(C);
	
	return 0;

}
