#include<stdio.h>
#include<string.h>
int  main(void){
	char wor[] = {'h', 'e', 'l', 'l', 'o', '\0'};
	printf("%d, %s\n", wor, wor);
	char wor2[] = {'h', 'e', 'l', 'l', 'o', '\0'};
	printf("%d, %s\n", wor2, wor2);
	return 0;
}
