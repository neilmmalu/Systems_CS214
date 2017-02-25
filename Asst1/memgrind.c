//#include "mymalloc.c"
#include<stdlib.h>
#include<stdio.h>
int main(int argc, char** argv){
	char *test[1000];
	// Part A
	int i=0;
	for(i=0;i<1000;i++){
		test[i] = (char*)malloc(sizeof(char));
	}
	for(i=0;i<1000;i++){
		free(test[i]);
	}
	//Part B
	int i=0;
	for(i=0;i<1000;i++){
		test[i] = (char*)malloc(sizeof(char));
		free(test[i]);
	}
	// Part C
	int k=0;
	while(k<1000){
		int r = rand() %2;
	switch(r){
		case 0:
			test[k] = (char*)malloc(sizeof(char));
			k++;
			break;
		case 1:
			if(k!=0){
			free(test[k-1]);
			}
			break;
		default:
			break;
	}
		free(test);
	}
	// Part D
	int k=0;
	while(k<1000){
		int r = rand() %2;
	switch(r){
		case 0:
			int bytes = rand() %65;
			break;
			//check if space is not being used already
			//test[somenumber] = malloc(bytes) amount of bytes
		case 1:
			//free the first pointer it can find
			break;
		default:
			break;
	}
	



	return 0;
	}

