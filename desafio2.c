#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int THREAD_NUM = 10;

void* thrd(){
	int i, vetor[100];
	for(i=0; i<100; i++)
		vetor[i] = i;
	pthread_exit(NULL);
}

int main() {
	int i;
	pthread_t *threads;

	threads = malloc(THREAD_NUM*sizeof(pthread_t*));

	for(i=0; i<THREAD_NUM; i++)
		pthread_create(&threads[i], NULL, thrd, NULL);

	for(i=0; i<THREAD_NUM; i++)
		pthread_join(threads[i], NULL);

	return 0;
}
