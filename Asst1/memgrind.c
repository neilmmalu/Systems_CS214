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
	// Part B
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
		//free(test);
	}	

	



	return 0;
	}

