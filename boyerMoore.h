
#define NO_OF_CHARS 256
#define MAX(A, B) ((A < B) ? B : A)

void boyerMooreInvoke(const char *source, const char *pattern);
void boyerMooreKernel(const char *source, unsigned int sLen, const char *pattern, unsigned int pLen, int *horspoolTable);
void horspool(const char *pattern, unsigned int pLen, int *horspoolTable);
void GPU_boyerMoore(const char *source,const char *pattern);
__global__ void GPU_boyerMoore_Kernel (int *d_table, char *d_source, int sourceLen, char *d_pattern, int pattenLen);