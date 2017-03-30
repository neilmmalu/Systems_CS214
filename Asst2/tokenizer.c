#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include "navigation.c"

Node* tokenize(FILE* file, char* fName)
{
    Node* head = NULL;
    char stream[5000];
    int i = 0;
	Node* temp = NULL;
	char c = fgetc(file);
	while (c != EOF)
    {
		i = 0;
		//skip all non-alphanumeric garbage THIS NEEDS TO SKIP ON FIRST NUMBERS TOO APPARENTLY
		while (c!=EOF && !isalpha(c))
		{
			c = fgetc(file);
		}
		//get all alphanumerics in current token
		while(c!= EOF && isalnum(c))
		{
			stream[i] = c; //does this move the pointer every time?
			i++;
			c = fgetc(file);
		}
		//make sure string is null terminated
		stream[i] = '\0';
		char* token = malloc(sizeof(char)*i + 1);
		if(strlen(stream)>0)
        {
		//+1?
			memcpy(token, stream, strlen(stream) + 1);
			if(head == NULL)
       	 	{
            	head = createNode(fName, token);
            	temp = head;
        	}
        	else
        	{
            	temp->next = createNode(fName, token);
            	temp = temp->next;
        	}
    	}
	}
	toLowerCase(head);
    return head;
}

Node* createNode(char* file, char* token)
{
    Node* n = (Node*)calloc(1, sizeof(Node));
    //mallocs and copies data into new string
    n->file = strdup(file);
    n->count = 1;
    n->token = token;
    n->next = NULL;
    return n;
}

hashTable* createHashTable(int size)
{
    hashTable* mainTable = (hashTable*)malloc(sizeof(hashTable));
    mainTable -> table = (Node**)malloc(sizeof(Node*)*size);
    int i;
    for (i=0; i<size; i++)
    {
        mainTable->table[i] = NULL;
    }
    mainTable->length = size;
    return mainTable;
}


//collects tokens, scatters into individual hash tables, and outputs them to designated output file
void outputTokens(hashTable* mainTable, FILE* mainOutputFile)
{
	int i;
	Node* head;
	Node* temp;
	Node* prev;
	char* token;
	int limit;
	
	for (i=0; i<mainTable->length; i++)
	{
		head = mainTable->table[i];
		while(head!=NULL)
		{
			temp = head;
			prev = temp;
			token = head->token;
			limit = temp->count;

			while (temp!=NULL && sortalnum(token, temp->token) ==0)
			{
				if(temp->count > limit)
				{
					limit = temp->count;
				}
				prev = temp;
				temp = temp->next;
			}
			
			mainTable -> table[i] = temp;
			prev->next = NULL;
			if(head!=NULL)
			{
				scatterTokens(head, limit, mainOutputFile);
			}
			head = mainTable->table[i];
		}
	}
	//deleteTable(mainTable);
    fprintf(mainOutputFile, "</fileIndex>\n");
}
//I'm like 99% sure this works
void scatterTokens(Node* head, int size, FILE* mainOutputFile)
{	
	Node *curr, *prev;//, *toFree;
//	toFree = head;
	hashTable* mainTable = createHashTable(size);
	while (head!=NULL)
	{
		if(mainTable->table[head->count-1]==NULL)
		{
			Node* temp = createNode(head->file, head->token);
			temp->count = head -> count;
			mainTable->table[head->count-1] = temp;
		}
		else
		{
			curr = mainTable->table[head->count-1];
			prev = curr;
			//for the same token with the same counts for different files, keep alphanumeric order
			while(curr!=NULL && strcmp(curr->file, head->file)<0)//sortalnum(curr->file, head->file)>0)
			{
				prev = curr;
				curr = curr->next;
			}
			Node* temp = createNode(head->file, head->token);
			temp->count = head->count;
			temp->next = curr;
			if (mainTable->table[temp->count-1] == curr)	
			{
				mainTable->table[temp->count-1] = temp;
			}
			else
			{
				prev->next = temp;
			}
	
		}
		head = head->next;
	}
	//deleteList(toFree);
	outputTokenList(mainTable, mainOutputFile);
}

void outputTokenList (hashTable* mainTable, FILE* mainOutputFile)
{
	boolean wordInitialized = FALSE;
	if (!outputInitialized)
	{
		fprintf(mainOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		fprintf(mainOutputFile, "<fileIndex>\n");
		outputInitialized = TRUE;
	}
	int i;
	Node* curr;
	for (i=0; i<mainTable->length; i++)
	{
		curr = mainTable->table[i];
		while(curr!=NULL)
		{
			if(!wordInitialized)
			{
				fprintf(mainOutputFile, "\t<word text = \"%s\">\n", curr->token);
				wordInitialized = TRUE;
			}
			//while(sortalnum(currTok, curr->token)==0)
			while (curr!=NULL)
			{
				fprintf(mainOutputFile, "\t\t<file name = \"%s\">%i</file>\n",curr->file, curr->count);
				curr = curr-> next;
			}
		}
	}
	fprintf(mainOutputFile, "\t</word>\n");
	//deleteTable(mainTable);
}
// void printOpeningTags(FILE* mainOutputFile)
// {
// 	fprintf(mainOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
// 	fprintf(mainOutputFile, "<fileIndex>\n");
// }

// void printClosingTags(FILE* mainOutputFile)
// {
// 	fprintf(mainOutputFile, "</fileIndex>\n");
// }
void toLowerCase(Node* head)
{
	int i;
	Node* temp = head;
	while(temp!=NULL)
	{	
		for (i=0; i<strlen(temp->token); i++)
		{
			if (isupper(temp->token[i]))
			{
				temp->token[i] = tolower(temp->token[i]);
			}
		}
		temp = temp->next;
	}
}

//modified strncmp to allow for alphanumerics
int sortalnum(const char *a, const char *b)
{
	while(*a != '\0' && *b != '\0')
		{
    
        	if (*a == *b)
			{
        	    a++;
        	    b++;
        	    continue;
        	}
        
        	if( (isalpha(*a) && isalpha(*b)) || (isdigit(*a) && isdigit(*b)) )
			{
            
        		if(*a < *b)
				return 1;
                
            		if(*b < *a)
                		return -1;
        	}
        	else if(isalpha(*a) && isdigit(*b))
			{
            		return 1;
        	}    
        	else
			{       //should be isdigit(*a) && isalpha(*b)
            		return -1;
        	}    
    	}
	//both pointers have matched until one hits null terminator
	if(*a == '\0' && *b == '\0')
	{
	//both pointers matched and are same length
		return 0;
	}
	else if(strlen(a) < strlen(b)){
	//first string is shorter
		return 1;
	}	
	else
	{
	//first string is longer
		return -1;
	}
}

void printTable(hashTable* hTable)
{
    int i;
    Node* curr;
    for (i=0; i<hTable->length; i++)
    {
        curr = hTable->table[i];
		while(curr!=NULL)
        {
            printf("token: %s   count: %i   fileName: %s\n", curr->token, curr->count, curr->file);
            curr = curr->next;
        }
    }
}

//free all inner nodes and table itself
// void deleteTable(hashTable* hTable)
// {
//     int i;
//     Node* curr;
//     Node* temp;
// 	for (i=0; i<hTable->length; i++)
//     {
//         curr = hTable->table[i];
//         while(curr!=NULL)
//         {
//             temp = curr->next;
//             free(curr->token);
// 			free(curr->file);
//             free(curr);
//             curr = temp;   		
// 		}
//     }
//     free(hTable->table);
//     free(hTable);
// }
//free unsorted temp linked list
// void deleteList(Node* head)
// {
//     Node* temp;
// 	while(head!=NULL)
//     {
//         temp = head->next;
//         free(head->token);
// 		free (head->file);
//         free(head);
//         head = temp;
//     }

// }

void printLL(Node* head)
{
	//Node* ptr = head;
	while(head != NULL)
	{
		printf("%s, %s\n", head->token, head ->file);
		head = head->next;
	}
}

int exists(char** argv)
{
	int temp = 1;
	char file [260];
	file[0] = '.';
	file[1] = '/';
	memcpy(file + 2, argv[1], strlen(argv[1])+1);
	printf("%s\n", file);
	if(access(file, F_OK) == 0)
	{
		printf("File already exists in directory. Do you wish you overwrite it? Enter 1 to proceed or 0 to exit\n");
	temp = getchar();
	}
	return temp;
}




