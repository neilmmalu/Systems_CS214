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
	Node *N;
	N->token="NeilMalu";
	T.next=N;
	printf("%s\n", T.token);
	return 0;
	
}

