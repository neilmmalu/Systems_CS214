#include "mymalloc.h"

int isEmpty = 1;
// int freeCount = 0;
// int mallocCount = 0;
// int cleanUpCount = 0;
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
	if(p == NULL){
		cleanUp();
		p = allocateMem(size, file, line);
	}
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
    	// printf("tracker: %hi\n", tracker);
    	blockSize = *(short *)current;
    	// printf("blockSize: %hi\n", blockSize);
    	if(blockSize > 0 && blockSize >= (short)size){
    		*(short *)current = (short)size *(-1);
    		// printf("current: %hi\n", *(short *)current);
    		// printf("blockSize: %hi\n", blockSize);

    		*(short *)(current + (short)size + 2) = blockSize - (short)size - 2;
    		// printf("mallocCount: %d\n", ++mallocCount);
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
    cleanUp();

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

	char *targetFree = p - 2;
	char *temp = myblock;
	short x = 0;

	while(targetFree != temp && x < 5000){
		
		short blockSize = *(short *)temp;
		// printf("blockSize: %hi\n", blockSize);
		if(blockSize < 0){
			blockSize *= -1;
		}

		x += blockSize + 2;
		temp += blockSize + 2;
	}

	if(targetFree == temp){
		if(*(short *)temp < 0){
			// freeCount++;
			// printf("freeCount: %d\n", ++freeCount);
			*(short *)temp *= -1;
			cleanUp();
			return;
		}
	}
}

void cleanUp(){
	char *ptr = myblock;
	short index = 0;
	// printf("cleanUpCount: %d\n", ++cleanUpCount);
	while(index < 5000){
		short blockSize = *(short *)ptr;
		// printf("blockSize: %hi\n", blockSize);
		if(blockSize > 0){
			char *ptr2 = (ptr + blockSize + 2);
			short blockSize2 = *(short *)ptr2;
			if(blockSize2 > 0){
				*(short *)ptr += *(short *)ptr2 + 2;
				*ptr2 = '0';
				// printf("cleanUpCount: %d\n", ++cleanUpCount);
				// printf("ptr: %hi\n", *(short *)ptr);
				return;
			}
			else{
				index += blockSize + blockSize2 + 4;
			}
		}
		else{
			index += blockSize + 2;
		}
	}
}