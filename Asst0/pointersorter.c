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

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Invalid use. Usage string:\t ./pointersorter \"This is valid input\"\n");
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
	head->strLength = NULL;
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
			// printf("j is : %d\n",j);
			word = (char *)realloc(word, (j+1)*sizeof(char));
			printf("Word: %s\n", word);
		}
		else{
			word[j] = '\0';
			char *temp = word;
			if(head->componentString == NULL){
				printf("Enter\n");
				head->componentString = temp;
				head->strLength = j;
				head->next = NULL;
			}
			else{
				
				struct Node *n = (struct Node*)malloc(sizeof(struct Node));
				n->componentString = temp;
				n->strLength = j;
				n->next = head;
				head = n;
			}
			j = 0;
			word = (char *)realloc(word, (j+1)*sizeof(char));
		}
		printf("head word: %s\n", head->componentString);
	}
	// printList(head);
}

void printList(struct Node* head){
	while(head != NULL){
		printf("%s -> ", head->componentString);
		head = head->next;
	}
	printf("\n");
}

