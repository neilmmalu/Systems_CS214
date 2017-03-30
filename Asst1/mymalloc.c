#include "mymalloc.h"

int isEmpty = 1; 

/*Add 0's or garbage values to the initial array and set the start as metadata to 4998*/
void initializeBlock(){
	int i = 0;
	for(i = 0; i < 5000; i++){
		myblock[i] = '0';
	}
	*(short *)myblock = 4998;
	isEmpty = 0;
}

void *mymalloc(int size, char *file, int line){
	//initialize empty array
	if(isEmpty){
		initializeBlock();
	}

	//allocate memory with requested size
	void *p = allocateMem(size, file, line);
	
	//If malloc fails, clean up and try again
	if(p == NULL){
		cleanUp();
		p = allocateMem(size, file, line);
	}
	return p;
}

void *allocateMem(int size, char* file, int line){

    //if requested size is bigger than size of heap
    if (size > 5000){
    	fprintf(stderr, "ERROR: Not enough free space in heap for malloc in FILE: %s LINE: %d \n", file, line);
    	return NULL;
    }

    //set 4999 and 5000 to 4998
    if(size > 4998){
    		size = 4998;
    	}

    //invalid input size
    if(size <= 0){
    	fprintf(stderr, "ERROR: Memory request for malloc must be greater that 0 in FILE: %s LINE: %d \n", file, line);
    	return NULL;
    }

    //pointer to the heap
    char *current = myblock;
    short blockSize;
    short tracker = 0;

    //loop through the entire block to search for space
    while(tracker < 5000){
    	
    	//size of the current block
    	blockSize = *(short *)current;
    	
    	//if the block is free and size of block is big enough for space requested
    	if(blockSize > 0 && blockSize >= (short)size){
    		//set the block as occupied
    		*(short *)current = (short)size *(-1);
    		//set an adjecent free block with the adjusted size
    		*(short *)(current + (short)size + 2) = blockSize - (short)size - 2;
    		return (void *) (current + 2);
    	}
    	
    	//If block is negative make it positive for calculations
    	if(blockSize < 0){
    		blockSize *= -1;
    	}
    	//set current to next metadata
    	current+= (blockSize + 2);
    	//add blocksize to tracker to reduce time taken to serach free blocks
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

	//If pointer points to NULL
	if(p == NULL){
		fprintf(stderr, "ERROR: Invalid pointer in FILE: %s at LINE: %d\n", file, line );
		return;
	}
	//pointer to metadata
	char *targetFree = p - 2;
	char *temp = myblock;
	short x = 0;

	//loop to go through the block
	while(targetFree != temp && x < 5000){
		//type cast and get blocksize from metadata
		short blockSize = *(short *)temp;
		//if block size is negative, make positive
		if(blockSize < 0){
			blockSize *= -1;
		}

		x += blockSize + 2;
		temp += blockSize + 2;
	}

	//check if pointers are equal
	if(targetFree == temp){
		//Check if the metadata is negative (or the space is allocated)
		if(*(short *)temp < 0){
			*(short *)temp *= -1;
			//clean up after freeing to prevent fragmentation
			cleanUp();
			return;
		}
	}
}

void cleanUp(){
	//pointer to block
	char *ptr = myblock;
	short index = 0;
	//loop to go through the block
	while(index < 5000){
		short blockSize = *(short *)ptr;
		if(blockSize > 0){
			//set new block pointer to the next metadata
			char *ptr2 = (ptr + blockSize + 2);
			short blockSize2 = *(short *)ptr2;
			//check if the next block is free and merge
			if(blockSize2 > 0){
				*(short *)ptr += *(short *)ptr2 + 2;
				*ptr2 = '0';
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
