#include "mymalloc.h"

int isEmpty = 1;

void initializeBlock(){
	int i = 0;
		for(i = 0; i < 5000; i++){
			myblock[i] = '0';
		}
		*(short *)myblock = 4998;
		isEmpty = 0;
}

void *mymalloc(int size, char *file, int line){
	if(isEmpty){
		initializeBlock();
	}

	void *p = allocateMem(size, file, line);

	return p;
}

void *allocateMem(int size, char* file, int line){
    int index = 0;
    int i;
    i = 0;

    //too small or too big
    if (size > 5000){
    	fprintf(stderr, "ERROR: Not enough free space in heap for malloc in FILE: %s LINE: %d \n", file, line);
    	return NULL;
    }

    if(size > 4998){
    		size = 4998;
    	}

    if(size <= 0){
    	fprintf(stderr, "ERROR: Memory request for malloc must be greater that 0 in FILE: %s LINE: %d \n", file, line);
    	return NULL;
    }

    char *current = myblock;
    short blockSize;
    short tracker = 0;
    while(tracker < 5000){
    	printf("tracker: %hi\n", tracker);
    	blockSize = *(short *)current;
    	printf("blockSize: %hi\n", blockSize);
    	if(blockSize > 0 && blockSize >= (short)size){
    		*(short *)current = (short)size *(-1);
    		printf("current: %hi\n", *(short *)current);
    		printf("blockSize: %hi\n", blockSize);
    		*(short *)(current + (short)size + 2) = blockSize - (short)size - 2;
    		return (void *) (current + 2);
    	}
    	// printf("blockSize: %hi\n", blockSize);
    	if(blockSize < 0){
    		blockSize *= -1;
    	}
    	current+= (blockSize + 2);
    	// printf("current: %hi\n", *(short *)current);
    	tracker+= blockSize;
    }

    fprintf(stderr, "ERROR: Not enough free space in heap for malloc in FILE: %s LINE: %d \n", file, line);
    return NULL;
}

void myfree(void *p, char *file, int line){
	if(isEmpty){
		initializeBlock();
	}

	if(p == NULL){
		fprintf(stderr, "ERROR: Invalid pointer in FILE: %s at LINE: %d\n", file, line );
		return;
	}

	char *temp = (char *)p;
	if(temp < &myblock[2] || temp > &myblock[4998]){
		fprintf(stderr, "ERROR: Invalid pointer in FILE: %s at LINE: %d\n", file, line );
		return;
	}

	char *curr = myblock;
	while(curr <= (temp - 2)){
		if(curr == (temp - 2)){
			if(!*(short *)curr & ~1){
				*(short *)curr *= -1;
				cleanUp();
				return;
			}
			else{
				fprintf(stderr, "ERROR: Pointer points to free memory in FILE: %s at LINE: %d\n", file, line );
				return;
			}
		}
		if((short)curr < 0){
			curr += *(short *)curr * (-1);
		}
		else{
			curr += *(short *)curr;
		}
	}

	fprintf(stderr, "ERROR: Invalid pointer in FILE: %s at LINE: %d\n", file, line );
	return;
}

void cleanUp(){
	char *temp = myblock;
	short size1 = *(short *)temp;
	while(temp < &myblock[4998]){
		size1 = *(short *)temp;
		if(size1 < 0){
			size1 *= -1;
			if((temp + size1) < &myblock[4998]){
				char *nextBlock = (temp + size1);
				short size2 = *(short *)nextBlock;
				if(size2 < 0){
					*(short *)temp *= -1;
					*(short *)nextBlock = 0;
					*(short *)temp += size2 + 2; 
				}
				else{
					return;
				}
			}
			else{
				return;
			}
			
		}
		temp += (2 + *(short *)temp + size1);
	}
}