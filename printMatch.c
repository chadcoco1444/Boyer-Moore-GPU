
#include "printMatch.h"


void printInfo(const char *source,const char *pattern){
	int i;
	for(i = 0; i < 70; i++) printf(ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET);	printf("\n");
	
	printf(ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET); for(i = 0; i < 68; i++) printf(" "); printf(ANSI_COLOR_YELLOW"*\n"ANSI_COLOR_RESET);
	
	printf(ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET);
	for(i = 0; i < 14; i++) printf(" ");
	printf(ANSI_COLOR_GREEN"String-matching : Boyer-Moore algorithm "ANSI_COLOR_RESET);
	for(i = 0; i < 14; i++) printf(" ");
	printf(ANSI_COLOR_YELLOW"*\n"ANSI_COLOR_RESET);	
	
	printf(ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET); for(i = 0; i < 68; i++) printf(" "); printf(ANSI_COLOR_YELLOW"*\n"ANSI_COLOR_RESET);

	for(i = 0; i < 70; i++) printf(ANSI_COLOR_YELLOW"*"ANSI_COLOR_RESET);	printf("\n");

	printf("Searching pattern ");
	printf(ANSI_COLOR_MAGENTA"\"%s\""ANSI_COLOR_RESET,pattern); printf(" in sourceFile\n\n");
	//printf(ANSI_COLOR_CYAN"\n%s\n\n"ANSI_COLOR_RESET,source);


}

void printMatch(const char *source, unsigned int pLen, const int *matBuf, unsigned int matTimes){
	int i;
	int j;
	int k;
	printf("Pattern found at Source marked: \n");
	for(i = 0; i < strlen(source); i++){
		for(k = 0; k < matTimes; k++){
			if(i == matBuf[k]){
				for(j = 0; j < pLen; j++)
					printf(ANSI_COLOR_RED "%c"ANSI_COLOR_RESET, source[i++]);
			}
		}
		printf(ANSI_COLOR_CYAN"%c"ANSI_COLOR_RESET, source[i]);

	}
	printf("\n");

}

