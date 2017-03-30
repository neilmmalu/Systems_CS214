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
					traverseDirectory(mainTable, path); 
				}
				break;
			}

			case DT_REG:
			{	
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
	if(closedir(directory)){
		printf("error could not close dir");
		return;
	}
}

void insertNode(Node* head, hashTable* list , char* file)
{
	
	int count =0;
	int i;
	char first;
	while(head!=NULL)
	{
		count++;
		first = head->token[0];
		i = first;
		if (!isalpha(first))
		{
			i += 26;
		}
		else
		{
			i -=97;
		}
		Node* n = createNode(file, head->token);
		if (list->table[i] == NULL || sortHelper(list->table[i]->token, n->token)<0)    
		{
			n->next = list->table[i];
			list->table[i] = n;
		}
		else
		{
			Node* curr = list->table[i];
			Node* prev = curr;
			while(curr != NULL && sortHelper(curr->token, n->token) > 0)
			{
				prev = curr;
				curr = curr->next;
			}
			if (curr!=NULL && sortHelper(curr->token, n->token) == 0)
			{
				if (strcmp(curr->file, n->file) != 0)
				{
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

void deleteTable(hashTable* list)
{
    int i;
    Node* head;
    Node* temp;
	for (i=0; i<list->length; i++)
    {
        head = list->table[i];
        while(head != NULL)
        {
            temp = head->next;
            free(head->token);
			free(head->file);
            free(head);
            head = temp;   		
		}
    }
    free(list->table);
    free(list);
}