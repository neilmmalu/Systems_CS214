#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenizer.h"
#include <errno.h>
#include <dirent.h>
#include <unistd.h>

void traverseDirectory(hashTable* mainTable, const char *directoryName)
{

	DIR* directory;
	FILE* fp;

	//printf("%s\t %d\n", directoryName, sizeof(directoryName));
	directory = opendir(directoryName);
	if(!directory)
	{
		if (errno == ENOTDIR)
		{
			char buffer[256];
			memcpy(buffer, directoryName, sizeof(directoryName) + 1);
			buffer[sizeof(directoryName)+1] = '\0';
			fp = fopen(directoryName, "r");
			if (!fp)
			{
				printf("file is null\n");
			}
			//printf("%s\n", buffer);
			Node* tokens = tokenize(fp, buffer);
			insertNode(tokens, mainTable, buffer);
			return;
		}
		else
		{
			printf("Error: could not open %s - File or directory  may not exist\n ",directoryName);
			return;
		}
	}
	while(!directory)
	{

		struct dirent* pwd;
		char* currentDirectory;
		pwd = readdir(directory);
		if(!pwd)
		{
			break;
		}
		currentDirectory = pwd->d_name;
		
		switch(pwd->d_type)
		{
			
			case DT_DIR:
			{
				if(strcmp(currentDirectory,".") != 0 && strcmp(currentDirectory, "..") != 0)
				{
					//need to EXTEND THE PATH for next traverseDirectory call, working directory doesn't change (think adir/ -> adir/bdir/....)
					int pathlength = 0;	
					char path[256];
					pathlength = snprintf(path, 256, "%s/%s",directoryName, currentDirectory);
					if(pathlength > 255)
					{
						printf("Path length is too long error");
						return;
					}
					traverseDirectory(mainTable, path); //RECURSIVE STEP
				}
				break;
			}
			case DT_REG:
				//regular files, need to check to ensure ".txt"....
			{	
				char pathname [256];
				FILE* fp;
				sprintf(pathname, "%s/%s", directoryName, currentDirectory);
				fp = fopen(pathname, "r");
				if (fp!=NULL)
				{
					Node* tmp = tokenize(fp, currentDirectory);	
					insertNode(tmp, mainTable, currentDirectory);
				}
				break;
			}
			default:
				// printf("something is not right in ur switch statement");
				return;
		}
	
	}
//	printf("closing directory: %s\n", directoryName); //DEBUGGING 
	if(closedir(directory)){
		printf("error could not close dir");
		return;
	}
}

Node* createNode(char* file, char* token)
{
    Node* head = (Node*)calloc(1, sizeof(Node));
    //mallocs and copies data into new string
    head->file = strdup(file);
    head->token = token;
    head->count = 1;
    head->next = NULL;
    return head;
}

hashTable* createHashTable(int size)
{
    hashTable* mainTable = (hashTable*)malloc(sizeof(hashTable));
    mainTable->table = (Node**)malloc(sizeof(Node*) * size);
    int i;
    for (i = 0; i < size; i++)
    {
        mainTable->table[i] = NULL;
    }
    mainTable->length = size;
    return mainTable;
}

//create a linked list where each node represents one token
Node* tokenize(FILE* file, char* fileName)
{
    Node* head = NULL;
    char buffer[10000];
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
		//get all alphanumerics in tempent token
		while(c!= EOF && isalnum(c))
		{
			buffer[i] = c; //does this move the pointer every time?
			i++;
			c = fgetc(file);
		}
		//make sure string is null terminated
		buffer[i] = '\0';
		char* token = malloc(sizeof(char)*i);
		if(strlen(buffer)>0)
        {
		//+1?
			memcpy(token, buffer, strlen(buffer));
			if(head == NULL)
       	 	{
            	head = createNode(fileName, token);
            	temp = head;
        	}
        	else
        	{
            	temp->next = createNode(fileName, token);
            	temp = temp->next;
        	}
    	}
	}
	toLowerCase(head);
    return head;
}
//works for determined edge cases. will run more scenarios

int validateInput(int argc)
{
    //too few or too many inputs
	//printf("%i\n", argc);
	if(argc!=3)
        {
		   printf("usage: ./tokenize output_file target_file/directory \n");
            return 1;
        }
    return 0;
}

void insertNode(Node* list, hashTable* hTable , char* fileName)
{
	int count =0;
    //slot in the hashTable according to leading letter
    int index;
    //leading letter
    char leading;
    //hashTable* hTable = createHashTable(36);
	while(list!=NULL)
	{
		count ++;
		leading = list->token[0];
        	index = leading;
		//alphas first in table, numerics second
		if (!isalpha(leading))
			{
				index += 26;
			}
		else
			{
				index -=97;
			}
		//node to be inserted
        Node* node = createNode(fileName, list->token);
        //if node is to be inserted at front of list
		if (hTable->table[index] == NULL || sortalnum(hTable->table[index]->token, node->token)<0)    
			{
                node->next = hTable->table[index];
                hTable->table[index] = node;
			}
        //if node is second node or later
            else
            {
                Node* curr = hTable->table[index];
                Node* prev = curr;
            //while string to be inserted comes after existing strings
                while(curr!=NULL && sortalnum(curr->token, node->token)>0)
                {
                    prev = curr;
                    curr = curr->next;
                }
			if (curr!=NULL && sortalnum(curr->token, node->token)==0)
			{
				if (strcmp(curr->fileName, node->fileName)!=0)
				{
								//HERE BEGINS THE NEW TERRITORY
							if(curr->next != NULL && strcmp(curr->next->token, node->token)==0 && strcmp(curr->next->fileName, node->fileName)!=0){
									curr->next->count++;
							//free(node);
							}else{
							//printf("original file %s, current file  %s, token = %s\n", curr->fileName, node->fileName, node->token);
							node->next = curr->next;
							curr -> next = node;
							//printHashTable(hTable);
							}
						}
						else
						{
							curr -> count ++;
							//free(node);
							printf("%s\n", curr->token);
						}
					}
					else
						{
						node->next = curr;
						prev->next = node;
						}
				}
				Node* temp = list;
				list = list->next;
				free(temp);
			}
    return;
}

//collects tokens, scatters into individual hash tables, and outputs them to designated output file
void printTokens(hashTable* masterTable, FILE* outputFile)
{
	int i;
	Node* head;
	Node* curr;
	Node* prev;
	char* currTok;
	int maxNum;
	
	for (i=0; i<masterTable->length; i++)
	{
		head = masterTable->table[i];
		while(head!=NULL)
		{
			curr = head;
			prev = curr;
			currTok = head->token;
			maxNum = curr->count;

			while (curr!=NULL && sortalnum(currTok, curr->token) ==0)
			{
				if(curr->count > maxNum)
				{
					maxNum = curr->count;
				}
				prev = curr;
				curr = curr->next;
			}
			
			if (curr!= NULL)
			{
				Node* temp = createNode(curr->fileName, curr->token);
				temp -> count = curr->count;
				temp -> next = curr -> next;
				masterTable -> table[i] = temp;
			}
			else
			{
				masterTable -> table[i] = NULL;
			}
			prev->next = NULL;
			if(head!=NULL)
			{
				scatterTokens(head, maxNum, outputFile);
			}
			head = masterTable->table[i];
		}
	}
	deleteHashTable(masterTable);
    printClosingTag(outputFile);
}
//I'm like 99% sure this works
hashTable* scatterTokens (Node* head, int size, FILE* outputFile)
{
	Node *curr, *prev, *toFree;
	toFree = head;
	hashTable* mainTable = createHashTable(size);
	while (head!=NULL)
	{
		if(mainTable->table[head->count-1]==NULL)
		{
			Node* temp = createNode(head->fileName, head->token);
			temp->count = head -> count;
			mainTable->table[head->count-1] = temp;
		}
		else
		{
			curr = mainTable->table[head->count-1];
			prev = curr;
			//for the same token with the same counts for different files, keep alphanumeric order
			while(curr!=NULL && strcmp(curr->fileName, head->fileName)<0)//sortalnum(curr->fileName, head->fileName)>0)
			{
				prev = curr;
				curr = curr->next;
			}
			Node* temp = createNode(head->fileName, head->token);
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
	deleteLinkedList(toFree);
	printTokenList(mainTable, outputFile);
}

void printTokenList (hashTable* mainTable, FILE* outputFile)
{
	boolean wordInitialized = FALSE;
	if (!outputInitialized)
	{
		printOpeningTag(outputFile);
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
				fprintf(outputFile, "\t<word text = \"%s\">\n", curr->token);
				wordInitialized = TRUE;
			}
			//while(sortalnum(currTok, curr->token)==0)
			while (curr!=NULL)
			{
				fprintf(outputFile, "\t\t<file name = \"%s\">%i</file>\n",curr->fileName, curr->count);
				curr = curr-> next;
			}
			fprintf(outputFile, "\t</word>\n");
		}
	}
	deleteHashTable(mainTable);
}
//not sure if i have to pass argv here or not...feel like no
void printOpeningTag(FILE* outputFile)
{
	fprintf(outputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(outputFile, "<fileIndex>\n");
}

void printClosingTag(FILE* outputFile)
{
	fprintf(outputFile, "</fileIndex>");
}
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

void printHashTable(hashTable* hTable)
{
    int i;
    Node* curr;
    for (i=0; i<hTable->length; i++)
    {
        curr = hTable->table[i];
		while(curr!=NULL)
        {
            printf("token: %s   count: %i   fileName: %s\n", curr->token, curr->count, curr-> fileName);
            curr = curr->next;
        }
    }
}

//free all inner nodes and table itself
void deleteHashTable(hashTable* hTable)
{
    int i;
    Node* curr;
    Node* temp;
	for (i=0; i<hTable->length; i++)
    {
        curr = hTable->table[i];
        while(curr!=NULL)
        {
            temp = curr->next;
            free(curr->token);
			free(curr->fileName);
            free(curr);
            curr = temp;   		
		}
    }
    free(hTable->table);
    free(hTable);
}
//free unsorted temp linked list
void deleteLinkedList(Node* head)
{
    Node* temp;
	while(head!=NULL)
    {
        temp = head->next;
        free(head->token);
		free (head->fileName);
        free(head);
        head = temp;
    }

}

void printLL(Node* head)
{
	//Node* ptr = head;
	while(head != NULL)
	{
		printf("%s, %s\n", head->token, head ->fileName);
		head = head->next;
	}
}

int exists(char** argv)
{
	int temp = 1;
	char buffer[256];
	buffer[0] = '.';
	buffer[1] = '/';
	memcpy(buffer+2, argv[1], strlen(argv[1])+1);
	printf("%s\n", buffer);
	if(access(buffer, F_OK) == 0)
	{
		printf("File already exists in directory. Do you wish you overwrite it? Enter 1 to proceed or 0 to exit\n");
	temp = getchar();
	}
	return temp;
}

