#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int *frogsPosition, *canJump; //vetores de controle
int rocks; //num de pedras
int deadCounter; //contador de quantas tentativas falhas sucessivas de pulo teve
int DEAD; //indica deadlock
int waitCreation; //Flag para esperar a criação de todas as threads antes de
				  //começar a execução  
int emptyStone; //frogsPosition[emptyStone] indica qual a pedra livre


pthread_mutex_t jumpMutex;

void imprimeLagoa() {
	int i, j;
	for(i=0; i<rocks; i++) {
		for (j=0; j<rocks; j++) {
			if (i == frogsPosition[j]) {
				if (j!= rocks-1)
					printf("%d, ", j);
				else
					printf("_, ");
			}
		}
	}
	printf("\n");

}

void *t_anfibio(void *arg) {
	int i, aux;
	int id = *(int*)arg;
	pthread_mutex_unlock(&jumpMutex);
	while (waitCreation == 0) {}

	while(!DEAD){//o programa ainda ta rodando
		pthread_mutex_lock(&jumpMutex); //LOCK
		
		if(canJump[id]) {//checa se pode pular
			deadCounter = 0;// Pedra livre e o sapo trocam de posições
			aux = frogsPosition[id];
			frogsPosition[id] = frogsPosition[emptyStone];//pula
			frogsPosition[emptyStone] = aux;

			for(i=0; i<rocks-1; i++)
				canJump[i] = 0;
		}
		else
			deadCounter++;
		pthread_mutex_unlock(&jumpMutex); //UNLOCK
	}
	pthread_exit(NULL);
}

void criaLagoa(int N, pthread_t *threads) {
	int i, id;
	DEAD = 0; //DEAD indica deadlock
	deadCounter = 0;
	waitCreation = 0; // esta flag é para todas as threads esperarem a criacão terminar
					  // tava pensando em usar um mutex aqui. 

	frogsPosition = malloc((rocks)*sizeof(int));//frogsPosition[i] indica qual o local do sapo de id=1
	canJump = malloc((rocks-1)*sizeof(int));//vetor a ser checado para cada animal se ele pode ou nao pular

	for(i=0; i < rocks-1; i++)
		canJump[i] = 0;

	pthread_mutex_init(&jumpMutex, NULL);
	for (i=0; i<N; i++) {
		pthread_mutex_lock(&jumpMutex);
		id = i;
		pthread_create(&threads[i], NULL, t_anfibio, &(id));
	   	frogsPosition[i] = i;

	}
	for (i=N+1; i<rocks; i++) {
		pthread_mutex_lock(&jumpMutex);
		id = i-1; // indica o id dos sapos
		pthread_create(&threads[id], NULL, t_anfibio, &(id));
		frogsPosition[id] = i;
	}
	frogsPosition[emptyStone] = N; // A pedra vazia começa sendo a pedra do centro

	waitCreation = 1;
	return;
}

void arbitro (int N) {
	int i, cantJump;
	while(!DEAD){
		cantJump = 1;
		pthread_mutex_lock(&jumpMutex); //LOCK
		for(i=0; i<rocks; i++){
			if (i<N){ //ra
				if (frogsPosition[i] +1 == frogsPosition[emptyStone] || frogsPosition[i]+2 == frogsPosition[emptyStone]){
					cantJump = 0;	
					canJump[i] = 1;
			   	}
			}
			else{ // sapo
				if (frogsPosition[i] -1 == frogsPosition[emptyStone] || frogsPosition[i]-2 == frogsPosition[emptyStone]) {
					cantJump = 0;
					canJump[i] = 1;
		   		}
			}
		}
		pthread_mutex_unlock(&jumpMutex); //UNLOCK

		if(cantJump || deadCounter > 200000) 
			DEAD = 1; //deadlock
		//to pensando se vale a pena ter um sleep aqui pra garantir que alguns sapos/ras chequem se podem pular
	}
	return;
}

int confereResultado(int N) {
	int i, isCorrect;
	isCorrect = 1;
	for(i=0; i<rocks-1; i++){ //checa a corretude do resultado
		if(i < N && frogsPosition[i] <= N-1) { //sapos deveriam estar todos a frente da M-esima+1 pedra
			isCorrect = 0;
			break;
		}
		if(i >= N && frogsPosition[i] >= N+1) { //ras deveriam estar todas atras da M-esima pedra
			isCorrect = 0;
			break;
		}
	}
	return isCorrect;
}

int main(int argv, char** argc) {
	int i, cont = 0;
	pthread_t *threads;
	clock_t start, end;
	
	start = clock();
	// Perguntaram no paca e é para ter somente um argumento
	if(argv != 2) {
		printf("Número de argumentos incorreto.\n");
		return 0;
	}
	rocks = atoi(argc[1]); // Numero de pedras no lago
	int N = (rocks-1)/2; // Quantidade de sapos na lago
	emptyStone = rocks-1; // A pedra vazia é indicada por frogsPosition[emtyStone]

	while(1) {
		threads = malloc((rocks-1)*sizeof(pthread_t*));//vetor das threads, se i < N é ra, cc é sapo
		criaLagoa(N, threads);

		arbitro(N);

		for(i=0; i<rocks-1; i++) //espera as threads juntarem
			pthread_join(threads[i], NULL);	

		if(confereResultado(N)) 
			break;
		else 
			cont++;

		//imprimeLagoa();
		free(threads);
		free(frogsPosition);
		free(canJump);
		pthread_mutex_destroy(&jumpMutex);
	}

	printf("%d resultados incorretos até funcionar\n", cont);
	printf("Estado final da lagoa:\n");
	imprimeLagoa();
	end = clock();
	printf("execucao durou %.2f segundos\n", (float)(end-start)/CLOCKS_PER_SEC);

	free(threads);
	free(frogsPosition);
	free(canJump);
	pthread_mutex_destroy(&jumpMutex);


	return 0;
}
