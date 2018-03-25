#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Biblioteca da função sleep

int *frogsPosition, *canJump; //vetores de controle
int rocks; //num de pedras
int deadCounter; //contador de quantas tentativas falhas sucessivas de pulo teve
int DEAD; //indica deadlock
int waitCreation; //Flag para esperar a criação de todas as threads antes de
                  //começar a execução  
int emptyStone; //frogsPosition[emptyStone] indica qual a pedra livre


pthread_mutex_t jumpMutex;

void *t_anfibio(void *arg) {
	int i, aux, id = *(int *)arg;
	while (waitCreation == 0) {}

	while(!DEAD){//o programa ainda ta rodando
		pthread_mutex_lock(&jumpMutex); //LOCK
		if(canJump[id]) {//checa se pode pular
			deadCounter = 0;
            		// Pedra livre e o sapo trocam de posições
            		aux = frogsPosition[id];
			frogsPosition[id] = frogsPosition[emptyStone];//pula
        	    	frogPosition[emptyStone] = aux;
			for(i=0; i<rocks; i++)
				canJump[i] = 0;
		}
		else
			deadCounter++;
		pthread_mutex_unlock(&jumpMutex); //UNLOCK
	}
	pthread_exit(NULL);
}


int main(int argv, char** argc) {
	int i, aux, id;
	int deadFlag, isCorrect;
	pthread_t *threads;

	DEAD = 0; //DEAD indica deadlock
	deadCounter = 0;

	// Perguntaram no paca e é para ter somente um argumento
	if(argv != 2) {
		printf("Número de argumentos incorreto.\n");
		return 0;
	}
	rocks = atoi(argc[1]); // Numero de pedras no lago
	int N = (rocks-1)/2; // Quantidade de sapos na lago
	emptyStone = rocks-1; // A pedra vazia é indicada por frogsPosition[emtyStone]

	//frogsPosition[i] indica qual o local do sapo de id=1
	frogsPosition = malloc((rocks)*sizeof(int));
	//vetor das threads, se i<N é ra, cc é sapo
	threads = malloc((rocks-1)*sizeof(pthread_t));
	//vetor a ser checado para cada animal se ele pode ou nao pular
	canJump = malloc((rocks-1)*sizeof(int));

	for(i=0; i < rocks; i++)
		canJump[i] = 0;

	pthread_mutex_init(&jumpMutex, NULL);
	waitCreation = 0; // esta flag é para todas as threads esperarem a criacão terminar
                      // tava pensando em usar um mutex aqui.  
	for (i=0; i<N; i++) {
        	pthread_create(&threads[i], NULL, t_anfibio, &(i));
        	frogsPosition[i] = i;
	}
	for (i=N+1; i<rocks; i++) {
        	id = i-1; // indica o id dos sapos
        	pthread_create(&threads[id], NULL, t_anfibio, &(id));
        	frogsPosition[id] = i;
    	}
    	frogsPosition[emptyStone] = N; // A pedra vazia começa sendo a pedra do centro
    	waitCreation = 1;

	while(!DEAD){
		deadFlag = 1;
		pthread_mutex_lock(&jumpMutex); //LOCK
		for(i=0; i<rocks; i++){
			if (i<N){ //ra
                		if (frogsPosition[i] +1 == frogsPosition[emptyStone] || frogsPosition[i]+2 == frogsPosition[emptyStone]){
    					deadFlag = 0;	
    					canJump[i] = 1;
                		}
			}else{ // sapo
                		if (frogsPosition[i] -1 == frogsPosition[emptyStone] || frogsPosition[i]-2 == frogsPosition[emptyStone]) {
    					deadFlag = 0;
    					canJump[i] = 1;
                		}
			}
		}
		pthread_mutex_unlock(&jumpMutex); //UNLOCK

		if(deadFlag || deadCounter > 999) DEAD = 1; //deadlock

		//to pensando se vale a pena ter um sleep aqui pra garantir que alguns sapos/ras chequem se podem pular
	}

	for(i=0; i<rocks; i++) //espera as threads juntarem
		pthread_join(threads[i], NULL);


	isCorrect = 1;
	for(i=0; i<rocks; i++){ //checa a corretude do resultado
		if(i < N && frogsPosition[i] <= N+1) { //sapos deveriam estar todos a frente da M-esima+1 pedra
			isCorrect = 0;
			break;
		}
		if(i >= N && frogsPosition[i] > N+1) { //ras deveriam estar todas atras da M-esima pedra
			isCorrect = 0;
			break;
		}
	}

	free(threads);
	free(frogsPosition);
	free(canJump);
	pthread_mutex_destroy(&jumpMutex);

	if(isCorrect)
		printf("O resultado é correto.\n");
	else
		printf("O resultado é incorreto!\n");

	return 0;
}
