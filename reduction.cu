#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <cuda_runtime.h>
#include <helper_functions.h>

int *S; //N matrizes 3x3
int *min;

//9 blocos, cada um d N/2 threads

__global__ void findMin(int N) {
	int tId = threadIdx.x;
	int bId = blockId.x;

	//A cada round toda thread tem um "companheiro" na outra metade de round. Desse modo todo elemento é checado
	for(unsigned int round=N/2; round>0; round/=2) {
		if(tId < round) {
			if(S[tId + bId] > S[tId+round*9+bId])
				S[tId+bId] = S[tId+round*9+bId];
		}

		if(tId == 0 && round%2 != 0){
			if(S[tId + bId] > S[tId+round*9+bId + 1])
				S[tId+bId] = S[tId+round*9+bId];
		}
		

		_syncthreads();
	}
	if(tId == 0)
		min[bId] = S[bId];

}

int main(int argc, char **argv) {
	FILE *lista;
	int numMatrix;
	int threadsPerBlock, blockNum;

	//inicializa
	if(argc != 2) {
		printf("Número errado de argumentos!!\n");
		return 1;
	}

	lista = fopen(argv[1], "r");

	fscanf(lista, "%d", &numMatrix);

	cudaMallocManaged(&S, 9*numMatrix*sizeof(int));
	cudaMallocManaged(&min, 9*sizeof(int));


	//executa
	threadsPerBlock = N/2;
	blockNum = 9;

	findMin<<< blockNum, threadsPerBlock >>>(numMatrix);
	cudaDeviceSynchronize();

	//free
	cudaFree(S);
	cudaFree(min);

	return 0;
}