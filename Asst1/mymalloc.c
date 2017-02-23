#include 'mymalloc.h'

int main(int argc, char *argv[]){

}

void *initializeBlock(char* myblock){
	*(short*) myBlock = 4998;
	for (i = 1; i < 2500; i++)
	{
		*(short*)(myBlock + i) = 0;
	}
}

void *mymalloc(int size){
	
}