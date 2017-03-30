#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include "tokenizer.h"

Node* tokenize(FILE* file, char* fileName)
{
    Node* head = NULL;
    char buffer[5000];
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
			buffer[i] = c; //does this move the pointer every time?
			i++;
			c = fgetc(file);
		}
		//make sure string is null terminated
		buffer[i] = '\0';
		char* token = malloc(sizeof(char)*i+1);
		if(strlen(buffer)>0)
        {
		//+1?
			memcpy(token, buffer, strlen(buffer)+1);
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


void traverseDirectory(hashTable* mainTable, const char * directoryName)
{

	DIR * directory;
	FILE* file;

	//printf("%s\t %d\n", directoryName, sizeof(directoryName));
	directory = opendir(directoryName);
	if(!directory)
	{
		if (errno == ENOTDIR)
		{
			char buffer[256];
			memcpy (buffer, directoryName, sizeof(directoryName)+1);
			buffer[sizeof(directoryName)+1] = '\0';
			file = fopen(directoryName, "r");
			if (file == NULL)
			{
				printf("file is null\n");
			}
			//printf("%s\n", buffer);
			Node* token = tokenize(file, buffer);
			insertNode(token, mainTable, buffer);
			return;
		}
		else
		{
			printf("Error: could not open %s - File or directory may not exist\n ", directoryName);
			return;
		}
	}
	while(directory != NULL)
	{
		struct dirent *pointer;
		char *pwd;
		pointer = readdir(directory);
		if(!pointer)
		{
			//end of stream, break
			break;
		}
		pwd = pointer->d_name;
		
		switch(pointer->d_type)
		{
			
			case DT_DIR:
			{
				if(strcmp(pwd,".") != 0 && strcmp(pwd, "..") != 0)
				{
					//need to EXTEND THE PATH for next traverseDirectory call, working dir doesn't change (think adir/ -> adir/bdir/....)
					int pathlength = 0;	
					char path[256];
					pathlength = snprintf(path, 256, "%s/%s",directoryName, pwd);
					if(pathlength > 255)
					{
						printf("Path length is too long error");
						return;
					}
					//strcat(path, pwd); //lengthens path 
					//printf("%s\n",pwd); //error checking and DEBUGGING
					traverseDirectory(mainTable, path); //RECURSIVE STEP
				}
				break;
			}
			case DT_REG:
				//regular files, need to check to ensure ".txt"....
			{	
				char pathname [256];
				FILE* fp;
				sprintf(pathname, "%s/%s", directoryName, pwd);
				fp = fopen(pathname, "r");
				if (fp!=NULL)
				{
					Node* n = tokenize(fp, pwd);	
					insertNode(n, mainTable, pwd);
				}
				break;
			}
			default:
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
    Node* n = (Node*)calloc(1, sizeof(Node));
    //mallocs and copies data into new string
    n->fileName = strdup(file);
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

int checkInput(int argc)
{
    //too few or too many inputs
	//printf("%i\n", argc);
	if(argc!=3)
        {
		   printf("usage: pointersorter.c output_file target_file/directory \n");
            return 1;
        }
    return 0;
}

//collects tokens, scatters into individual hash tables, and outputs them to designated output file
void outputTokens(hashTable* masterTable, FILE* mainOutputFile)
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
			
				masterTable -> table[i] = curr;
			prev->next = NULL;
			if(head!=NULL)
			{
				scatterTokens(head, maxNum, mainOutputFile);
			}
			head = masterTable->table[i];
		}
	}
	//deleteTable(masterTable);
    printClosingTags(mainOutputFile);
}
//I'm like 99% sure this works
void scatterTokens (Node* head, int size, FILE* mainOutputFile)
{	
	Node *curr, *prev;//, *toFree;
//	toFree = head;
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
	//deleteList(toFree);
	outputTokenList(mainTable, mainOutputFile);
}

void outputTokenList (hashTable* mainTable, FILE* mainOutputFile)
{
	boolean wordInitialized = FALSE;
	if (!outputInitialized)
	{
		printOpeningTags(mainOutputFile);
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
				fprintf(mainOutputFile, "\t\t<file name = \"%s\">%i</file>\n",curr->fileName, curr->count);
				curr = curr-> next;
			}
		}
	}
	fprintf(mainOutputFile, "\t</word>\n");
	//deleteTable(mainTable);
}
void printOpeningTags(FILE* mainOutputFile)
{
	fprintf(mainOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(mainOutputFile, "<fileIndex>\n");
}

void printClosingTags(FILE* mainOutputFile)
{
	fprintf(mainOutputFile, "</fileIndex>\n");
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

void printTable(hashTable* hTable)
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
void deleteTable(hashTable* hTable)
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
void deleteList(Node* head)
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
	int x = 1;
	char file [260];
	file[0] = '.';
	file[1] = '/';
	memcpy(file+2, argv[1], strlen(argv[1])+1);
	printf("%s\n", file);
	if(access(file, F_OK) == 0)
	{
		printf("File already exists in directory. Do you wish you overwrite it? Enter 1 to proceed or 0 to exit\n");
	x = getchar();
	}
	return x;
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
		if (hTable->table[index] == NULL || sortalnum(hTable->table[index]->token, node->token)<0)    		{
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
							if(curr->next != NULL && strcmp(curr->next->token, node->token)==0 && strcmp(node->fileName, curr->next->fileName)==0)
							{
								//printf("well it might be working");	
								curr->next->count++;
							}
							else
							{
								node->next = curr->next;
								curr->next=node;
							}
				}
							else
							{
								curr->count ++;
							}
				}
				else
				{
					node->next = curr;
					prev->next = node;
				}
			}
			list = list->next;
			}
			return;
	}


int main(int argc, char** argv)
{
	hashTable* mainTable = createHashTable(36);

	if(argc!=3)
    {
	  	printf("usage: ./index output_file target_file/directory \n");
        return 1;
    }
	if (exists(argv)==0)
	{
		return 0;
	}
	traverseDirectory(mainTable, argv[2]);
	FILE* mainOutputFile = fopen(argv[1], "w+");
	outputTokens(mainTable, mainOutputFile);
	return 0;
}


