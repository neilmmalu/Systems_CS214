#include "mymalloc.h"

int isEmpty = 1;


void initializeBlock(){
	char *p;
    p = &myblock[0];
    *((short*)p) = -5000;
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
	char *temp = myblock;
    int newSize = size + 2;
    int index = 0;
    
    int i;
    i = 0;

    //too small or too big
    if (size < 1 || size > 4998){
    	return NULL;
    }

    do {

        short *mallocSize = (short*)(temp);
        short mallocSizeMetadata = *mallocSize * -1;
        if (mallocSizeMetadata < 0){
            mallocSizeMetadata *= -1;
        }

        int margin = mallocSizeMetadata - newSize;
        if((*mallocSize < 0) && (margin == 0 || margin > 2)){
            
        	short *p;
            //sets size of allocated space
            p = (short*)temp;
            *p = newSize;
            
            //sets size of new unallocated block if block is split
            if (newSize != mallocSizeMetadata){
                p = (short*)(temp + newSize);
                *p = (mallocSizeMetadata - newSize) * -1; //negative because unallocated
            }
            return (temp + 2);
        }
        else{
            index = index + mallocSizeMetadata;
            temp = &myblock[index];
            i++;
        }
    }while (index < 5000);

    return NULL;
}