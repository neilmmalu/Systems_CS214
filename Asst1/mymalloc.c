#include "mymalloc.h"

int isEmpty = 1;

int main(int argc, char *argv[]){
	initializeBlock();
	int i = 0;
	for(i = 0; i < 5000; i++){
		printf("%c", myblock[i]);
	}
}

void initializeBlock(){
	char *p;
    p = &myblock[0];
    *((short*)p) = 4998;
}

void *mymalloc(int size, char *file, int line){
	if(isEmpty){
		initializeBlock();
		isEmpty = 0;
	}

	void *p = allocateMem(size);
	if(p == NULL){
		fprintf(stderr, "ERROR: Not enough free space for malloc in FILE: %s LINE: %d \n", file, line);
	}

	return p;
}

void *allocateMem(int size){
	if(size < 1 || size > 5000){
		return NULL;
	}
	if(size > 4998 && size < 5000){
		size = 4998;
	}
	char *temp = myblock;
	int newSize = size + 2;
	int index = 0;

	// do{

	// }while(index < 5000);
}