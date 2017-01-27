#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

struct Node{
	char *componentString;
	int strLength;
	struct Node *next;
}Node;

void printList(struct Node* head);

struct Node *addNode(struct Node *head, char *word, int j);

int main(int argc, char *argv[]){
	if(argc != 2){ 
		printf("Invalid use. Proper Usage:   ./pointersorter \"This1is2valid3input\"\n");
		//printf("Incorrect number of parameters: \n Usage: ./pointersorter \"astring\"\n");
		return 0;
	}
	char *input = argv[1]; 

	if(input == ""){
		printf("error\n");
		return 0;
	}
	int inputLength = strlen(input);

	struct Node *head = (struct Node*)malloc(sizeof(struct Node));
	head->componentString = NULL;
	head->strLength = 0; 
	head->next = NULL;
	char c;
	char *word = (char *)malloc(sizeof(char)); 
	int i = 0, j = 0;
	for(i = 0; i <= inputLength; i++){
		c = input[i];
		
		if(isalpha(c)){
			word[j] = c;
			j++;
			word[j] = '\0';
			word = (char *)realloc(word, (j+1)*sizeof(char));
		}
		else{
			word[j] = '\0';
			char *copy = (char *)malloc(sizeof(strlen(word) + 1));
			strcpy(copy, word);
			head = addNode(head, copy, j);
			j = 0;
			word = (char *)realloc(word, (j+1)*sizeof(char));
		}
	}
	printList(head);
}

void printList(struct Node *head)
{
	for(head = head; head != NULL; head = head->next)
	{
		printf("%s ", head->componentString);
		
		if(head->next != NULL)
		{
			printf("\t");
		}
	}
	printf("\n");
}


struct Node *addNode(struct Node *head, char *copy, int j){
	if(head->strLength == 0){
		head->componentString = copy;
		head->strLength = j;
		head->next = NULL;
		return head;
	}
	struct Node *n = (struct Node*)malloc(sizeof(struct Node));
	n->componentString = copy;
	n->strLength = j;
	n->next = head;
	head = n;
	return head;
}

