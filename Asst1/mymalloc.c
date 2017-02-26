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

    while(index < 5000){
    	short *mallocSize = (short*)(temp);
        short mallocSizeMetadata = *mallocSize * -1;
        if (mallocSizeMetadata <= 0){
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
            index += mallocSizeMetadata;
            temp = &myblock[index];
            i++;
        }
    }
    return NULL;
}

void myfree(void *p, char *file, int line){
	char * first = myblock;
    char * second = myblock;
    short* firstSize = (short*)first;
    int index = 0;
    int doesNotExist = 1;
    int check1 = 0;
    int check2 = 1;
    if (p < (void*)&myblock[0] || p > (void*)&myblock[4997]){
        fprintf(stderr, "ERROR: Invalid pointer in FILE: %s at LINE: %d\n", file, line );
        doesNotExist = 0;
    }
    
    while (index < 5000 && doesNotExist){
        if (p == first + 2){

            if(*firstSize < 0){           //already free
                //do nothing
                fprintf(stderr, "ERROR: Double free or pointer not allocated in FILE: %s at LINE: %d\n", file, line  );
                
            }

            if(first == second && (short*) second > 0){
            	check1 = 1;
            }
            else if((short *) second > 0){
            	check1 = 1;
            }

            char *next = &myblock[index + *firstSize];
            if((index + (short *) first) == 5000 || (short *)next > 0){
            	check2 = 0;
            }
            else if (check1 && check2){
                short *nextSize = (short*)next;
                short* secondSize = (short*)second;
                *secondSize = *secondSize - *firstSize + *nextSize;
                //printf("FREE SUCCESSFULL A\n");
            }
            else if (check1){
                //increases size of second block and keeps negative
                short* secondSize = (short*)second;
                *secondSize = *secondSize - *firstSize;
                //printf("FREE SUCCESSFULL B\n");
            }
            else if (check2){
                //increases size of first block and makes negative
                short *nextSize = (short*)next;
                *firstSize = *nextSize - *firstSize;
                //printf("FREE SUCCESSFULL C\n");
            }
            else{
                *firstSize = *firstSize * -1;
                //printf("FREE SUCCESSFULL D\n");
            }
            doesNotExist = 0;
        }
        else {
            second = first;
            if (*firstSize < 0){
                index += (*firstSize * -1);
            }
            else{
                index += *firstSize;
            }
            first = &myblock[index];
        }
    }
    if (doesNotExist == 1){
        fprintf(stderr, "ERROR: Double free or pointer not allocated in FILE: %s at LINE: %d\n", file, line);
    }

}