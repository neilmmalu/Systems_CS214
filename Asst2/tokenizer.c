#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include "utils.c"

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
		while (c!=EOF && !isalpha(c))
		{
			c = fgetc(file);
		}
		while(c!= EOF && isalnum(c))
		{
			stream[i] = c; 
			i++;
			c = fgetc(file);
		}
		stream[i] = '\0';
		char* token = malloc(sizeof(char)*i + 1);
		if(strlen(stream)>0)
        {
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
	
	Node *ptr = head;
	while(ptr != NULL)
	{	
		for (i = 0; i<strlen(ptr->token); i++)
		{
			if (isupper(ptr->token[i]))
			{
				ptr->token[i] = tolower(ptr->token[i]);
			}
		}
		ptr = ptr->next;
	}

    return head;
}

Node* createNode(char* file, char* token)
{
    Node* n = (Node*)calloc(1, sizeof(Node));
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
		while(head != NULL)
		{
			temp = head;
			prev = temp;
			token = head->token;
			limit = temp->count;

			while (temp!=NULL && sortHelper(token, temp->token) ==0)
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
				addTokens(limit, mainOutputFile, head);
			}
			head = mainTable->table[i];
		}
	}
    fprintf(mainOutputFile, "</fileIndex>\n");
}

void addTokens(int size, FILE* mainOutputFile, Node* head)
{	
	Node *curr, *prev;
	hashTable* mainTable = createHashTable(size);
	while (head != NULL)
	{
		if(mainTable->table[head->count - 1]==NULL)
		{
			Node* temp = createNode(head->file, head->token);
			temp->count = head->count;
			mainTable->table[head->count - 1] = temp;
		}
		else
		{
			curr = mainTable->table[head->count - 1];
			prev = curr;
			while(curr!=NULL && strcmp(curr->file, head->file)<0)
			{
				prev = curr;
				curr = curr->next;
			}
			Node* temp = createNode(head->file, head->token);
			temp->count = head->count;
			temp->next = curr;
			if (mainTable->table[temp->count - 1] == curr)	
			{
				mainTable->table[temp->count - 1] = temp;
			}
			else
			{
				prev->next = temp;
			}
	
		}
		head = head->next;
	}
	printTokens(mainTable, mainOutputFile);
}

void printTokens(hashTable* mainTable, FILE* mainOutputFile)
{
	int flag = 0;
	// if (!boolean)
	// {
	// 	fprintf(mainOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	// 	fprintf(mainOutputFile, "<fileIndex>\n");
	// 	boolean = 1;
	// }
	int i;
	Node* curr;
	for (i=0; i<mainTable->length; i++)
	{
		curr = mainTable->table[i];
		while(curr!=NULL)
		{
			if(!flag)
			{
				fprintf(mainOutputFile, "\t<word text = \"%s\">\n", curr->token);
				flag = 1;
			}
			while (curr!=NULL)
			{
				fprintf(mainOutputFile, "\t\t<file name = \"%s\">%i</file>\n",curr->file, curr->count);
				curr = curr-> next;
			}
		}
	}
	fprintf(mainOutputFile, "\t</word>\n");
}


//modified strncmp to allow for alphanumerics
int sortHelper(const char *a, const char *b)
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
		{       
			return -1;
		}    
	}
	if(*a == '\0' && *b == '\0')
	{
		return 0;
	}
	else if(strlen(a) < strlen(b))
	{
		return 1;
	}	
	else
	{
		return -1;
	}
}

int exists(char** argv)
{
	int temp = 1;
	char file [260];
	file[0] = '.';
	file[1] = '/';
	memcpy(file + 2, argv[1], strlen(argv[1]) + 1);
	if(access(file, F_OK) == 0)
	{
		printf("File already exists. Do you wish to continue? Enter 1 to proceed or 0 to exit\n");
		temp = getchar();
	}
	int flag;
	if(temp == 1)
	{
		flag = remove(file + 2);
		if(flag == 0)
		{
			printf("File deleted\n");
		}
	}
	return temp;
}




