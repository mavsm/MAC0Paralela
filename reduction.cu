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


	for(i=0;i<numMatrix*9; i+=9) {
		fscanf(lista, "");
		fscanf(lista, "%d %d %d", &S[i] &S[i+1] &S[i+2]);
		fscanf(lista, "%d %d %d", &S[i+3] &S[i+1+3] &S[i+2+3]);
		fscanf(lista, "%d %d %d", &S[i+6] &S[i+1+6] &S[i+2+6]);
	}


	//executa
	threadsPerBlock = numMatrix/2;
	blockNum = 9;

	findMin<<< blockNum, threadsPerBlock >>>(numMatrix);
	cudaDeviceSynchronize();

	for(i=0; i<3; i+=3)
		printf("%d %d %d\n", min[i] min[i+1] min[i+2])

	//free
	cudaFree(S);
	cudaFree(min);

	return 0;
}