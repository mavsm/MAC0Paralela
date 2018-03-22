#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *pond, *podePular;


void sapo(int id){
	while(){//o programa ainda ta rodando
		if(podePular[id])//checa se ṕode pular
			//pula
	}
}

void ra(int id){
	while(){//o programa ainda ta rodando
		if(podePular[id])//checa se ṕode pular
			//pula
	}
}


int main(int argv, char** argc) {
	int N, M, i, DEAD = 0;
	int *pond, *podePular;
	pthread_t *threads;

	if(argv == 3){
		N = atoi(argc[1]);
		M = atoi(argc[2]);
	}

	//guarda o estado da lagoa, que pedra esta ocupada por quem
	pond = malloc((N+M+1)*sizeof(int));
	//vetor das threads
	threads = malloc((N+M)*sizeof(threads));
	//vetor a ser checado para cada animal se ele pode ou nao pular
	podePular = malloc((N+M)*sizeof(int));

	for(i=0; i<N+M; i++)
		podePular[i] = 0;
	pond[N] = -1; // pedra central desocupada

	for(i=0; i<N+M; i++) {
		if(i<N) {
			pthread_create(&threads[i], sapo, NULL, i);
			pond[i] = i;
		}
		else{
			pthread_create(&threads[i], ra, NULL, i);
			pond[1+i] = i;
		}
	}

	while(!DEAD){
		for(i=0; i<N+M; i++){
			if(pond[i]<N && i<N+M-1) //sapo
				if(pond[i+1]==-1 || (i<N+M-2 && pond[i+2] == -1))
					podePular[pond[i]] = 1;
			else if(i>0) //ra
				if(pond[i-1]==-1 || (i>1 && pond[i-2] == -1))
					podePular[pond[i]] = 1;
		}
	}
	return 0;
}