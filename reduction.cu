#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cuda_runtime.h>


//9 blocos, cada um d N/2 threads

__global__ 
void findMin(int N, int *S, int *min) {
	int tId = threadIdx.x;
	int bId = blockIdx.x;

	//A cada round toda thread tem um "companheiro" na outra metade de round. Desse modo todo elemento é checado
	for(unsigned int round=N/2; round>1; round/=2) {
		if(tId < round) {
			if(S[tId + bId] > S[tId+round*9+bId])
				S[tId+bId] = S[tId+round*9+bId];
		}

		if(tId == 0 && round%2 != 0){
			if(S[tId + bId] > S[tId+round*9+bId + 1])
				S[tId+bId] = S[tId+round*9+bId];
		}
		printf("Sou %d e olhei para %d\ntId = %d, bId = %d, round = %d \n", tId + bId, tId+round*9+bId, tId, bId, round);

		__syncthreads();
	}
	if(tId == 0)
		min[bId] = S[bId];

}

int main(int argc, char **argv) {
	FILE *lista;
	int numMatrix, i;
	int *S; //N matrizes 3x3
	int *min;
	int threadsPerBlock, blockNum;
	char aux[3];

	//inicializa
	if(argc != 2) {
		printf("Número errado de argumentos!!\n");
		return 0;
	}
	lista = fopen(argv[1], "r");
	printf("Leu arquivo\n");

	fscanf(lista, "%d", &numMatrix);
	printf("%d\n", numMatrix);

	cudaMallocManaged(&S, 9*numMatrix*sizeof(int));
	cudaMallocManaged(&min, 9*sizeof(int));
	printf("Alocou memoria\n");

	for(i=0;i<numMatrix*9; i+=9) {
		fscanf(lista, "%s", aux);
		fscanf(lista, "%d %d %d", &S[i], &S[i+1], &S[i+2]);
		fscanf(lista, "%d %d %d", &S[i+3], &S[i+1+3], &S[i+2+3]);
		fscanf(lista, "%d %d %d", &S[i+6], &S[i+1+6], &S[i+2+6]);
	}
	printf("terminou leitura\n");

	//executa
	threadsPerBlock = numMatrix/2;
	blockNum = 9;

	//PARA RODAR NA REDE LINUX
	cudaSetDevice(0);

	findMin<<< blockNum, threadsPerBlock >>>(numMatrix, S, min);
	cudaDeviceSynchronize();

	for(i=0; i<3; i+=1) {
		printf("%d %d %d\n", min[i], min[i+1], min[i+2]);
	}

	//free
	cudaFree(S);
	cudaFree(min);

	return 0;
}