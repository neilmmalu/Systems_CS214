#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include "tokenizer.h"

void traverseDirectory(hashTable* mainTable, const char * directoryName)
{
	FILE* file;
	DIR* directory;

	//printf("%s\t %d\n", directoryName, sizeof(directoryName));
	directory = opendir(directoryName);
	if(!directory)
	{
		if (errno == ENOTDIR)
		{
			char buffer[256];
			memcpy(buffer, directoryName, sizeof(directoryName) + 1);
			buffer[sizeof(directoryName)+1] = '\0';
			file = fopen(directoryName, "r");
			if (file == NULL)
			{
				printf("null\n");
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
		char *pwd;
		struct dirent *pointer;
		pointer = readdir(directory);
		if(!pointer)
		{
			break;
		}
		pwd = pointer->d_name;
		
		switch(pointer->d_type)
		{
			
			case DT_DIR:
			{
				if(strcmp(pwd,".") != 0 && strcmp(pwd, "..") != 0)
				{
					int length = 0;	
					char path[256];
					length = snprintf(path, 256, "%s/%s",directoryName, pwd);
					if(length > 255)
					{
						printf("Path length is too long error");
						return;
					}
					traverseDirectory(mainTable, path); //RECURSIVE STEP
				}
				break;
			}

			case DT_REG:
			{	//regular files, need to check to ensure ".txt"....
				char pathname[256];
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

void insertNode(Node* head, hashTable* list , char* file)
{
	
	int count =0;
    //slot in the hashTable according to first letter
	int i;
    //leading letter
	char first;
    //hashTable* list = createHashTable(36);
	while(head!=NULL)
	{
		count++;
		first = head->token[0];
		i = first;
		//alphas first in table, numerics second
		if (!isalpha(first))
		{
			i += 26;
		}
		else
		{
			i -=97;
		}
		//node to be inserted
		Node* n = createNode(file, head->token);
        //if node is to be inserted at front of head
		if (list->table[i] == NULL || sortalnum(list->table[i]->token, n->token)<0)    
		{
			n->next = list->table[i];
			list->table[i] = n;
		}
        //if n is second node or later
		else
		{
			Node* curr = list->table[i];
			Node* prev = curr;
            //while string to be inserted comes after existing strings
			while(curr != NULL && sortalnum(curr->token, n->token) > 0)
			{
				prev = curr;
				curr = curr->next;
			}
			if (curr!=NULL && sortalnum(curr->token, n->token) == 0)
			{
				if (strcmp(curr->file, n->file) != 0)
				{
								//HERE BEGINS THE NEW TERRITORY
					if(curr->next != NULL && strcmp(curr->next->token, n->token)==0 && strcmp(n->file, curr->next->file)==0)
					{
						curr->next->count++;
					}
					else
					{
						n->next = curr->next;
						curr->next = n;
					}
				}
				else
				{
					curr->count++;
				}
			}
			else
			{
				n->next = curr;
				prev->next = n;
			}
		}
		head = head->next;
	}
	return;
}

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
			free(curr->file);
            free(curr);
            curr = temp;   		
		}
    }
    free(hTable->table);
    free(hTable);
}

void deleteList(Node* head)
{
    Node* temp;
	while(head!=NULL)
    {
        temp = head->next;
        free(head->token);
		free (head->file);
        free(head);
        head = temp;
    }

}