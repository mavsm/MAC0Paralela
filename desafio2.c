#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int THREAD_NUM = 10;
int creationFlag, vectorSize, numberOfThreads;
pthread_mutex_t creationMutex;

void *thrd(void *arg){
	int i, x;
	int *vector = *(int**)arg;
	x= vectorSize/numberOfThreads;

	while(creationFlag == 0);

	for (i=0; i<x; i++)
		vector[i]++;

<<<<<<< HEAD
=======
void* thrd(){
	int i, vetor[100];
	for(i=0; i<100; i++)
		vetor[i] = i;
>>>>>>> d430bf7dabe9798b870c493ce163ea7d70a07b6c
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
	cpu_set_t *cpuset;
	clock_t start, end;

	start = clock();
	if (argc < 3 ) {
		printf ("Argumentos não válidos\n");
		return 0;
	}
	vectorSize = atoi(argv[1]);
	numberOfThreads = atoi (argv[2]);

	CPUs = malloc(numberOfThreads*sizeof(int));
	for (i=0; i<numberOfThreads; i++) {
		scanf("%d", &CPUs[i]);
	}
	vectors = createVectors();

	cpuset = malloc(numberOfThreads*sizeof(cpu_set_t));
	for (i=0; i<numberOfThreads; i++)
        CPU_SET(CPUs[i], &cpuset[i]);

    pthread_mutex_init(&creationMutex, NULL);
    creationFlag = 0;
    threads = malloc(numberOfThreads*sizeof(pthread_t));
	for (i=0; i<numberOfThreads; i++) {
		//pthread_mutex_lock(&creationMutex);
		pthread_create(&threads[i], NULL, thrd, &(vectors[i]));
		pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset[i]);
		//pthread_mutex_unlock(&creationMutex);
	}
	creationFlag = 1;

	for(i=0; i<numberOfThreads; i++) //espera as threads juntarem
		pthread_join(threads[i], NULL);

	printf("%ld\n", clock()-start);





/*	threads = malloc(THREAD_NUM*sizeof(pthread_t));

	for(i=0; i<THREAD_NUM; i++)
		pthread_create(&threads[i], NULL, thrd, NULL);

	for(i=0; i<THREAD_NUM; i++)
		pthread_join(threads[i], NULL);*/

	return 0;
}
