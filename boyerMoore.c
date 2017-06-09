
#include "boyerMoore.h"
void boyerMooreInvoke(const char *source,const char *pattern){
#ifdef SMALL_PRINT
	
#endif

	int *horspoolTable;
    horspoolTable = (int *)calloc(NO_OF_CHARS, sizeof(int));

    horspool(pattern, strlen(pattern), horspoolTable);
	boyerMooreKernel(source, strlen(source), pattern, strlen(pattern), horspoolTable);
	free(horspoolTable);

}
void boyerMooreKernel(const char *source, unsigned int sLen, const char *pattern, unsigned int pLen, int *horspoolTable){

    

#ifdef SMALL_PRINT
    unsigned int matTimes = 0;
    int *matBuf;
    matBuf = (int *)calloc(256000000, sizeof(char));
#endif

    unsigned int matCnt = 0;
    unsigned int j, i = 0;

    while (i <= sLen - pLen)
    {
        j = pLen - 1;
        while (j > 0 && source[i + j] == pattern[j]) {
            --j;
        }
        if (j == 0) {

#ifdef SMALL_PRINT
            matBuf[matTimes++] = i;
#endif
            matCnt = i; 
            break;
            //i += (pLen - horspoolTable[source[i+j]]);
        } else {
        	i += MAX(1, j - horspoolTable[source[i+j]]);
    	}	
    }

printf(ANSI_COLOR_CYAN"CPU found at ["ANSI_COLOR_RED"%d"ANSI_COLOR_RESET ANSI_COLOR_CYAN"] position\n"ANSI_COLOR_RESET, matCnt);
#ifdef SMALL_PRINT
    printMatch(source, pLen, matBuf, matTimes);
#endif

}


void horspool(const char *pattern, unsigned int pLen, int *horspoolTable){

	int i;
    for (i = 0; i < NO_OF_CHARS; i++) {
        horspoolTable[i] = -1;
    }
    for (i = 0; i < pLen; i++) {
        horspoolTable[(int)pattern[i]] = i;
    }

}

void GPU_boyerMoore (const char *source, const char *pattern){
    
    int *horspoolTable = (int *)calloc(NO_OF_CHARS, sizeof(int));
    horspool(pattern, strlen(pattern), horspoolTable);

    size_t sizeTable = NO_OF_CHARS * sizeof(int);
    size_t sizeSource = strlen(source);
    size_t sizePattern = strlen(pattern);

    int *d_table;
    char *d_source, *d_pattern;

    cudaMalloc((void **)&d_table, sizeTable);
    cudaMemcpy(d_table, horspoolTable, sizeTable, cudaMemcpyHostToDevice);
    
    cudaMalloc((void **)&d_source, sizeSource);
    cudaMemcpy(d_source, source, sizeSource, cudaMemcpyHostToDevice);

    cudaMalloc((void **)&d_pattern, sizePattern);
    cudaMemcpy(d_pattern, pattern, sizePattern, cudaMemcpyHostToDevice);
    
    
    // Time 
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
    int n = strlen(source)/strlen(pattern);
    int threadsPerBlock = 1024;

    int blocksPerGrid = n / threadsPerBlock + (n % threadsPerBlock == 0 ? 0 : 1);
    // Invoke kernel
    GPU_boyerMoore_Kernel<<<blocksPerGrid, threadsPerBlock>>>(d_table, d_source, sizeSource, d_pattern, sizePattern);
    cudaError_t cuda_err = cudaGetLastError();
    if ( cudaSuccess != cuda_err ){
        printf("before kernel call: error = %s\n", cudaGetErrorString (cuda_err));
        exit(1) ;
    }

    // Time 
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    printf("GPU time: %.6f s\n", elapsedTime/1000);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    

    cudaDeviceSynchronize();
    int answer;
    cudaMemcpyFromSymbol(&answer, d_matCnt, sizeof(int), 0, cudaMemcpyDeviceToHost);
    printf(ANSI_COLOR_CYAN"GPU found at ["ANSI_COLOR_RED"%d"ANSI_COLOR_RESET ANSI_COLOR_CYAN"] position\n"ANSI_COLOR_RESET, answer);

    cudaFree(d_table);
    cudaFree(d_source);
    cudaFree(d_pattern);
}



__global__ void GPU_boyerMoore_Kernel (int *d_table, char *d_source, int sizeSource, char *d_pattern, int sizePattern){

    d_matCnt = 0;
    unsigned int sourceLen = sizeSource;
    unsigned int patternLen = sizePattern;
    unsigned int j, i = 0;  

    int tid = blockIdx.x * blockDim.x + threadIdx.x; 

    int N = sourceLen / patternLen;

    if(tid < N){

        int beg = tid * patternLen;
        int end = min(beg + ( 2 * patternLen), sourceLen);
        i = beg + patternLen - 1;

        while (i <= end) {
            j = patternLen - 1;
            while (j > 0 && d_source[i + j] == d_pattern[j]) {
                --j;
            }
            if (j == 0) {
                d_matCnt = i;
                break;
            } else {
                i += MAX(1, j - d_table[d_source[i+j]]);
            }   
        }

    }

}

