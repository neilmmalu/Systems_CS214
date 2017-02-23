#include<stdio.h>
#include<stdlib.h>

#define malloc(x) mymalloc(x)

void *mymalloc(int size);

static char myblock[5000];