#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

typedef struct Node
{
	char* fileName;
	int count;
	char* token;
	struct Node* next;
}Node;

typedef struct HashTable
{
	Node** table;
	int length;
}hashTable;

typedef enum {FALSE, TRUE} boolean;

char* inputString = "this is a test sentence now^Im utilizing $%^&symbols$%^&*(*&^$as+_)(*&delimiters This  o3ne has9876568798 nu289374mbers\0";

boolean outputInitialized = FALSE;

void insertNode(Node* list, hashTable* hTable , char* fileName);

void traverseDirectory(hashTable* hTable, const char * dir_name);

Node* createNode(char* fileName, char* token);

hashTable* makeHashTable(int size);

Node* tokenize(FILE* file, char* fileName);

int checkInput(int argc);

hashTable* makeMasterTable(Node* head, char* fileName);

void outputTokens(hashTable* masterTable, FILE* outputFile);

void scatterTokens(Node* head, int size, FILE* outputFile);

void outputTokenList(hashTable* myTable, FILE* outputFile);

void initializeOutput();

void closeOutput();

void myToLower(Node* head);

int sortalnum(const char* a, const char* b);

void printTable(hashTable* hTable);

void deleteTable(hashTable* hTable);

void destroyList(Node* head);

void printLL(Node* head);


