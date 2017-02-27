#include <stdio.h>
#include <stdlib.h>

#ifndef mymalloc_h
#define mymalloc_h
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

static char myblock[5000];

void *mymalloc(int size, char *file, int line);
void *allocateMem(int size, char *file, int line);
void myfree(void *p, char *file, int line);

void cleanUp();

void initializeBlock();

#endif