#include 'mymalloc.h'

int main(int argc, char *argv[]){

}

void *initializeBlock(char* myblock){
	short i = 1;
	*(short*) myblock = 4998;
	for (i = 1; i < 2500; i++)
	{
		*(short*)(myblock + i) = 0;
	}
}

void *mymalloc(int size, char *file, int line){
	if(myblock[0] < 0){

	}
}