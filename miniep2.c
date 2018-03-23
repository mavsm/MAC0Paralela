#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *pond, *podePular; //vetores de controle
int N, M; //num de sapos e ras, respectivamente
int deadCounter; //contador de quantas tentativas falhas sucessivas de pulo teve

void t_anfibio(int start, int id) {
	int local = start, i;
	while(){//o programa ainda ta rodando
		if(podePular[id]) {//checa se ṕode pular
			deadCounter = 0
			start = pond[N+M];
			pond[N+M] = local;//pula
			for(i=0; i<N+M; i++)
				podePular[i] = 0;
		}
		else
			deadCounter++;
		local = start;
	}
}


int main(int argv, char** argc) {
	int i, DEAD = 0; //DEAD indica deadlock
	int deadFlag;
	pthread_t *threads;

	deadCounter = 0;

	if(argv == 3){
		N = atoi(argc[1]);
		M = atoi(argc[2]);
	}

	//guarda o estado da lagoa, que pedra (pond[i]) esta ocupada por quem (i)
	pond = malloc((N+M+1)*sizeof(int));
	//vetor das threads, se i<N é sapo, cc é ra
	threads = malloc((N+M)*sizeof(threads));
	//vetor a ser checado para cada animal se ele pode ou nao pular
	podePular = malloc((N+M)*sizeof(int));

	for(i=0; i<N+M; i++)
		podePular[i] = 0;
	pond[N+M] = N; // pedra central desocupada

	for(i=0; i<N+M; i++) {
		if(i<N) { //sapo
			pthread_create(&threads[i], t_anfibio, NULL, i, i);
			pond[i] = i;
		}
		else{ //ra
			pthread_create(&threads[i], t_anfibio, NULL, i+1, i);
			pond[i] = i+1;
		}
	}

	while(!DEAD){
		deadFlag = 1;
		for(i=0; i<N+M; i++){
			if(pond[i] > pond[N+M]-2 && pond[i] < pond[N+M] && i<N){ //sapo pode pular
				deadFlag = 0;	
				podePular[i] = 1;
			}
			if(pond[i] < pond[N+M]+2 && pond[i] > pond[N+M] && i>=N){ // ra pode pular
				deadFlag = 0;
				podePular[i] = 1;
			}
		}
		if(deadFlag) DEAD = 1;
		if(deadCounter > 10 000) DEAD = 1;
	}
	return 0;
}