#include<stdio.h>
#include<string.h>
struct fNode{
char* token;
struct fNode* next;
}fNode;

typedef struct fNode Node;

int  main(void){
	char wor[] = {'h', 'e', 'l', 'l', 'o', '\0'};
	printf("%d, %s\n", wor, wor);
	char wor2[] = {'h', 'e', 'l', 'l', 'o', '\0'};
	printf("%d, %s\n", wor2, wor2);
	printf("Neil is Za Worst\n");
	printf("C is actually the worst language\n");
	Node T;	
	T.token= "Slavery";
	printf("%s\n", T.token);
	/*Something is wrong with declaring stuff
	char stuff[] = ('b', 'b', 'c');
	char *letter=stuff;
	printf("%s, %s, %s\n", stuff[0], stuff[1],stuff[2]);
	
	
	char *p= (char*)malloc(11*sizeof(char));
	Does some crazy shit, allocates memory so that this shit language can do things. Allocates memory in the form of chars
	*/
	return 0;
	
	
}

