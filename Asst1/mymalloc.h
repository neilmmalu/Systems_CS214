#include<stdio.h>
#include<stdlib.h>

#define malloc(x) mymalloc(x)
#define free(x) myfree(x)
static char myblock[5000];

void *mymalloc(int size);

void *myfree(char *p);

int cleanUp(char* myblock, int size);

