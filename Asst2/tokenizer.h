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

char* inputString = "this is a test sentence now^Im utilizing $%^&symbols$%^&*(*&^$as+_)(*&delimiters This  o3ne has9876568798 nu289374mbers\0";

int outputInitialized = 0;

void traverseDirectory(const char * directoryName, hashTable* list);

void insertNode(char* fileName, Node* head, hashTable* list);

Node* createNode(char* fileName, char* token);

hashTable* createHashTable(int size);

Node* tokenize(FILE* file, char* fileName);

hashTable* makeMasterTable(Node* head, char* fileName);

void printTokens(hashTable* masterTable, FILE* outputFile);

void addTokens(Node* head, int size, FILE* outputFile);

void outputTokenList(hashTable* myTable, FILE* outputFile);

int sortHelper(const char* a, const char* b);

void deleteTable(hashTable* list);

int exists(char** argv);

