#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

typedef struct Node
{
	char* file;
	char* token;
	int count;
	struct Node* next;
}Node;

typedef struct HashTable
{
	Node** table;
	int length;
}hashTable;

int boolean = 0;

void insertNode(Node* head, hashTable* list , char* fileName);

void traverseDirectory(hashTable* list, const char * directoryName);

Node* createNode(char* fileName, char* token);

hashTable* createHashTable(int size);

Node* tokenize(FILE* file, char* fileName);

void printTokens(hashTable* masterTable, FILE* outputFile);

void addTokens(Node* head, int size, FILE* outputFile);

void outputTokenList(hashTable* myTable, FILE* outputFile);

int sortHelper(const char* a, const char* b);

void deleteTable(hashTable* list);

int exists(char** argv);

