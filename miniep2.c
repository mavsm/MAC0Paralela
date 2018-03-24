#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *pond, *podePular; //vetores de controle
int N, M; //num de sapos e ras, respectivamente
int deadCounter; //contador de quantas tentativas falhas sucessivas de pulo teve
int DEAD; //indica deadlock


pthread_mutex_t pularMutex;

void t_anfibio(int start, int id) {
	int local = start, i;
	while(!DEAD){//o programa ainda ta rodando
		pthread_mutex_lock(&pularMutex); //LOCK
		if(podePular[id]) {//checa se ṕode pular
			deadCounter = 0
			start = pond[N+M];
			pond[N+M] = local;//pula
			for(i=0; i<N+M; i++)
				podePular[i] = 0;
		}
		else
			deadCounter++;
		pthread_mutex_unlock(&pularMutex); //UNLOCK
		local = start;
	}
	pthread_exit(NULL);
}


int main(int argv, char** argc) {
	int i;
	int deadFlag, isCorrect;
	pthread_t *threads;

	DEAD = 0; //DEAD indica deadlock
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

	pthread_mutex_init(&pularMutex, NULL);

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
		pthread_mutex_lock(&pularMutex); //LOCK
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
		pthread_mutex_unlock(&pularMutex); //UNLOCK

		if(deadFlag || deadCounter > 999) DEAD = 1; //deadlock

		//to pensando se vale a pena ter um sleep aqui pra garantir que alguns sapos/ras chequem se podem pular
	}

	for(i=0; i<N+M; i++) //espera as threads juntarem
		pthread_join(threads[i], NULL);


	isCorrect = 1;
	for(i=0; i<N+M; i++){ //checa a corretude do resultado
		if(i < N && pond[i] <= M+1) { //sapos deveriam estar todos a frente da M-esima+1 pedra
			isCorrect = 0;
			break;
		}
		if(i >= N && pond[i] >= M) { //ras deveriam estar todas atras da M-esima pedra
			isCorrect = 0;
			break;
		}
	}

	free(threads);
	free(pond);
	free(podePular);
	pthread_mutex_destroy(&pularMutex);

	if(isCorrect)
		printf("O resultado é correto.\n");
	else
		printf("O resultado é incorreto!\n");

	return 0;
}