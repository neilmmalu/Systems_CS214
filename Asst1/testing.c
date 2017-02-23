#include<stdlib.h>
#include<stdio.h>


int main(int argc, char** argv){
	char *array[100];
	int i=0;
	for(i=0;i<100;i++){
		array[i] = (char*)malloc(sizeof(char));
	}
	printf("%u \n", array[0]);
	free(array[0]);
	array[0]=NULL;
	printf("%u \n", array[0]);
	return 0;
	}
