#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
//#define SMALL_PRINT
#include "printMatch.c"
#include "boyerMoore.c"


int main(int argc, char const *argv[]){

	int sourceSize;

	FILE *source;
	FILE *pattern;
	source = fopen("../source512MB.txt", "rb");
	pattern = fopen("../pattern.txt", "rb");
	if(!source && !pattern){
		fprintf(stderr, ANSI_COLOR_RED"Cannot open the file\n"ANSI_COLOR_RESET);
		exit(1);
	}


	char *sBuffer;	//char sbuffer[1024];
	char *pBuffer;
	
	fseek(source, 0, SEEK_END);
    sourceSize = ftell(source);
    rewind(source);

    sBuffer = calloc(sourceSize, sizeof(char));
    pBuffer = calloc(1024, sizeof(char));


	fread( sBuffer, sourceSize, 1, source);
	fread( pBuffer, 1024, 1, pattern);

	clock_t begin, end, elapsed;
	begin = clock();
	boyerMooreInvoke(sBuffer, pBuffer);
	end = clock();
	printf("Execution time: %.3lf s\n", (double)(end - begin)/CLOCKS_PER_SEC);
	free(sBuffer);
	free(pBuffer);

	return 0;
}









