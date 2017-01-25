#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
	if(argc != 1){
		printf("Invalid use. Usage string:\t ./pointersorter \"This is valid input\" 			");
		return 0;
	}

	char *input = argv[0];

	if(input == ''){
		return 0;
	}
	
}
