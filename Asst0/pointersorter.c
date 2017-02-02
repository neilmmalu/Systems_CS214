#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

typedef struct Node{
	char *componentString;
	int strLength;
	struct Node *next;
} Node;

void printList(Node *head);

Node *addNode(Node *head, char *word, int j);

int main(int argc, char *argv[]){
	
	/*Validation for invalid parameters with usage statement*/
	if(argc != 2){ 
		printf("Invalid use. Proper Usage:   ./pointersorter \"This1is2valid3input\"\n");
		//printf("Incorrect number of parameters: \n Usage: ./pointersorter \"astring\"\n");
		return 0;
	}

	char *input = argv[1]; 
	/*Printing nothing if string is whitespaces*/
	int strCheck = strcmp(input, "");
	if(strCheck == 0){
		return 0;
	}

	/*Storing the length of the string*/
	int inputLength = strlen(input);

	/*Allocate memory to the first node and set it to default(NULLs and 0)*/
	Node *head = (Node*)malloc(sizeof(Node));
	head->componentString = NULL;
	head->strLength = 0; 
	head->next = NULL;

	/*Declare char to go through string*/
	char c;

	/*Allocate memory to store component strings*/
	char *word = (char *)malloc(sizeof(char)); 
	
	/*i is size of input length and j is length of each component string */
	int i = 0, j = 0;
	for(i = 0; i <= inputLength; i++){
		c = input[i];
		
		/*check if the char is an alphabet or something else*/
		if(isalpha(c)){
			/*Add the char to the word in position 'j'*/
			word[j] = c;
			j++;
			/*Add null char at the end of strings*/
			word[j] = '\0';
			/*reallocate memory to word so that another char can be added in 
			the next iteration*/
			word = (char *)realloc(word, (j+1)*sizeof(char));
		}
		else{
			
			/*If there's no letters between seperators, ignore them*/
			if(j == 0){
				continue;
			}

			word[j] = '\0';
			
			/*Make a copy of the word, so that when word changes, the nodes won't*/
			char *copy = (char *)malloc(sizeof(strlen(word) + 1));
			strcpy(copy, word);

			/*Add node to head and return the first node*/
			head = addNode(head, copy, j);
			j = 0;
			word = (char *)realloc(word, (j+1)*sizeof(char));
		}
	}
	printList(head);
	free(word);
	free(head);
	return 1;
}

/*Function to print the strings at the end*/
void printList(Node *head){
	if(head->componentString == NULL){
		return;
	}
	for(head = head; head != NULL; head = head->next)
	{
		printf("%s\n", head->componentString);
	}
}

/*Function to add node in appropriate position*/
Node *addNode(Node *head, char *copy, int j){
	
	/*Check if head is initialized and set head as first string if not */
	if(head->strLength == 0){
		head->componentString = copy;
		head->strLength = j;
		head->next = NULL;
		return head;
	}

	/*create new node */
	Node *n = (Node*)malloc(sizeof(Node));
	n->componentString = copy;
	n->strLength = j;

	/*check if head needs to be changed. i.e new node should be before head*/
	int strCheck = strcmp(n->componentString, head->componentString);
	
	/*if strCheck < 0, that means head changes*/
	if(strCheck < 0){
		n->next = head;
		head = n;
		return head;
	}

	/*set a pointer to go through the linked list*/
	Node *ptr = head;
	while(ptr->next != NULL){
		
		/*to check if n lies between ptr and ptr->next */
		strCheck = strcmp(n->componentString, ptr->next->componentString);
		if(strCheck < 0){
			/*Add n between ptr and ptr->next*/
			n->next = ptr->next;
			ptr->next = n;
			return head;
		}
		ptr = ptr->next;
	}
	
	/*If it doesn't go anywhere, it has to go at the end*/
	ptr->next = n;
	n->next = NULL;
	return head;
}

