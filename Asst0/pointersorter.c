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
	if(argc != 2){ /* Program always ends here, idk what argc!=2 means but the program never gets past here*/
		printf("Invalid use. Usage string:\t ./pointersorter \"This is valid input\"\n");
		//printf("Incorrect number of parameters: \n Usage: ./pointersorter \"astring\"\n");
		//this is some print that explains how to use the program to the user? He wrote it.
		return 0;
	}
	char *input = argv[1]; /* he saying something about dont change argv[1] Why.. he says youre toast*/

	if(input == ""){
		printf("error\n");
		return 0;
	}
	int inputLength = strlen(input);

	struct Node *head = (struct Node*)malloc(sizeof(struct Node));
<<<<<<< HEAD
	head->strLength = 0;
=======
	head->componentString = NULL;
	head->strLength = 0; //ints cant be null - compiles after this change
	head->next = NULL;
>>>>>>> refs/remotes/origin/master
	char c;
	char *word = (char *)malloc(sizeof(char)); //Says to initialize *word before doing malloc
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
			char *copy = (char *)malloc(sizeof(strlen(word) + 1));
			strcpy(copy, word);
			head = addNode(head, copy, j);
			j = 0;
			word = (char *)realloc(word, (j+1)*sizeof(char));
		}
		// printf("head word: %s\n", head->componentString);
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
		printf("In add node head: ");
		printList(head);
		return head;
	}
	struct Node *n = (struct Node*)malloc(sizeof(struct Node));
	n->componentString = copy;
	n->strLength = j;
	printf("In add node: ");
	printList(head);
	n->next = head;
	head = n;
	return head;
}

