#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

//#define SMALL_PRINT
#define CPU
#define GPU

__device__ int d_matCnt;

#include "../printMatch.c"
#include "../boyerMoore.c"
#include <cuda_runtime.h>


int main(int argc, char const *argv[]){

	int sourceSize, patternSize;

	FILE *source;
	FILE *pattern;
	source = fopen("../../source128MB.txt", "rb");
	pattern = fopen("../../pattern.txt", "rb");
	
	if(!source && !pattern){
		fprintf(stderr, ANSI_COLOR_RED"Cannot open the file\n"ANSI_COLOR_RESET);
		exit(1);
	}

	char *sBuffer;	
	char *pBuffer;
	
	fseek(source, 0, SEEK_END);
    sourceSize = ftell(source);
    rewind(source);

    fseek(pattern, 0, SEEK_END);
    patternSize = ftell(pattern);
    rewind(pattern);

    sBuffer = (char *)calloc(sourceSize, sizeof(char));
    pBuffer = (char *)calloc(patternSize, sizeof(char));

	fread( sBuffer, sourceSize, 1, source);
	fread( pBuffer, patternSize, 1, pattern);

	printInfo(sBuffer, pBuffer);

#ifdef CPU
	clock_t begin, end;
	begin = clock();
	boyerMooreInvoke(sBuffer, pBuffer);
	end = clock();
	printf("CPU time: %.6lf s\n", (double)(end - begin)/CLOCKS_PER_SEC);
#endif 

#ifdef GPU
	GPU_boyerMoore(sBuffer, pBuffer);
#endif

	free(sBuffer);
	free(pBuffer);

	return 0;
}









