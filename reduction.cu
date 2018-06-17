#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cuda_runtime.h>


//9 blocos, cada um d N/2 threads

__global__ 
void findMin(int N, int *S, int *min) {
	int tId = threadIdx.x; //thread ID
	int bId = blockIdx.x; //block ID

	int roundAnt = N;
	//A cada round toda thread tem um "companheiro" na outra metade de round. Desse modo todo elemento é checado
	for(unsigned int round=N/2; round>0; round/=2, roundAnt/=2) {
		if(tId < round) {
			if(S[tId*9 + bId] > S[(tId+round)*9+bId])
				S[tId*9+bId] = S[(tId+round)*9+bId];
		}

		if(tId == 0 && roundAnt%2 != 0){
			if(S[bId] > S[(roundAnt-1)*9+bId])
				S[bId] = S[(roundAnt-1)*9+bId];
		}
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

	fscanf(lista, "%d", &numMatrix);

	cudaMallocManaged(&S, 9*numMatrix*sizeof(int));
	cudaMallocManaged(&min, 9*sizeof(int));

	for(i=0;i<numMatrix*9; i+=9) {
		fscanf(lista, "%s", aux);
		fscanf(lista, "%d %d %d", &S[i], &S[i+1], &S[i+2]);
		fscanf(lista, "%d %d %d", &S[i+3], &S[i+1+3], &S[i+2+3]);
		fscanf(lista, "%d %d %d", &S[i+6], &S[i+1+6], &S[i+2+6]);
	}

	//executa
	threadsPerBlock = numMatrix/2;
	blockNum = 9;

	//PARA RODAR NA REDE LINUX
	//cudaSetDevice(0);

	findMin<<< blockNum, threadsPerBlock >>>(numMatrix, S, min);
	cudaDeviceSynchronize();

	printf("%d %d %d\n", min[0], min[1], min[2]);
	printf("%d %d %d\n", min[3], min[4], min[5]);
	printf("%d %d %d\n", min[6], min[7], min[8]);
		

	//free
	cudaFree(S);
	cudaFree(min);

	return 0;
}