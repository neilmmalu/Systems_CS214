#include "mymalloc.c"

int main(int argc, char** argv){
	char *test[1000];
	int i=0;
	for(i=0;i<1000;i++){
		test[i] = (char*)malloc(sizeof(char));
	}
	for(i=0;i<1000;i++){
		free(test[i]);
	}




	return 0;
	}

