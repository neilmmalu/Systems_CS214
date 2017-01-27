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
	
/*Validation for invalid parameters with usage statement*/
	if(argc != 2){ 
		printf("Invalid use. Usage string:\t ./pointersorter \"This is valid input\"\n");
		//printf("Incorrect number of parameters: \n Usage: ./pointersorter \"astring\"\n");
		return 0;
	}

	char *input = argv[1]; 
	/*Printing nothing if string is whitespaces*/
	int strCheck = strcmp(input, "");
	if(strCheck == 0){
		printf("");
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
			if(j == 0){
				continue;
			}
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
	if(head->componentString == NULL){
		printf("");
		return;
	}
	for(head = head; head != NULL; head = head->next)
	{
		printf("%s\n", head->componentString);
	}
}


struct Node *addNode(struct Node *head, char *copy, int j){
	/*Check if head is initialized and set head as first string if not */
	if(head->strLength == 0){
		head->componentString = copy;
		head->strLength = j;
		head->next = NULL;
		return head;
	}

	/*create new node */
	struct Node *n = (struct Node*)malloc(sizeof(struct Node));
	n->componentString = copy;
	n->strLength = j;

	/*check if head needs to be changed. i.e new node should be before head*/
	int strCheck = strcmp(head->componentString, n->componentString);
	/*if strCheck < 0, that means n precedes head*/
	if(strCheck > 0){
		n->next = head;
		head = n;
		return head;
	}

	/*set a pointer to go through linked list*/
	struct Node* ptr = head;
	while(ptr->next != NULL){
		/*to check if n lies between ptr and ptr->next */
		strCheck = strcmp(ptr->next->componentString, n->componentString);
		if(strCheck > 0){
			n->next = ptr->next;
			ptr->next = n;
			return head;
		}
		ptr = ptr->next;
	}

	ptr->next = n;
	n->next = NULL;
	return head;
}

