#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int creationFlag, vectorSize, numberOfThreads;

void *thrd(void *arg){
	int x = vectorSize/numberOfThreads;
	int *vector = *(int**)arg;

	while(creationFlag == 0);

	for (int i=0; i<x; i++)
		vector[i]++;	
	pthread_exit(NULL);
}

/* ./desafio2 a b
	a = tamanho do vetor
	b = quantas threads deseja usar
*/

int** createVectors() {
	int x, i;
	int **vectors;

	vectors = malloc(numberOfThreads*sizeof(int*));
	x = vectorSize/numberOfThreads;

	for (i=0; i<numberOfThreads; i++) 
		vectors[i] = malloc(x*sizeof(int));

	for (i=0; i<numberOfThreads; i++)
		vectors[i/x][i%x] = i;

	return vectors; 
	
}


int main (int argc, char *argv[]) {
	int i;
	int *CPUs, **vectors;
	pthread_t *threads;
	clock_t start;	
	cpu_set_t *cpuset;
	
	//Recebimento de argumentos
	if (argc < 3 ) {
		printf ("Argumentos não válidos\n");
		return 0;
	}
	vectorSize = atoi(argv[1]);
	numberOfThreads = atoi (argv[2]);

	//Alocação de espaço
	CPUs = malloc(numberOfThreads*sizeof(int));
	cpuset = malloc(numberOfThreads*sizeof(cpu_set_t));	
    threads = malloc(numberOfThreads*sizeof(pthread_t));

    //Criação das threads
	vectors = createVectors();
    creationFlag = 0;
	for (i=0; i<numberOfThreads; i++) {
		pthread_create(&threads[i], NULL, thrd, &(vectors[i]));
		// Recebe como input o numero da cpu a ser usada
		scanf("%d", &CPUs[i]);
		CPU_SET(CPUs[i], &cpuset[i]);
		pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset[i]);
	}

	start = clock();
	creationFlag = 1;

	for(i=0; i<numberOfThreads; i++) //espera as threads juntarem
		pthread_join(threads[i], NULL);
	printf("A execução demorou %ld clocks\n", clock()-start);

	return 0;
}
