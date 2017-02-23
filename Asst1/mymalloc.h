#include<stdio.h>
#include<stdlib.h>

#ifndef mymalloc_h
#define mymalloc_h
#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

static char myblock[5000];

void *mymalloc(int size, char *file, int line);

void *myfree(char *p, char *file, int line);

int cleanUp(char* myblock, int size);

void *initializeBlock(char* myblock);

#endif