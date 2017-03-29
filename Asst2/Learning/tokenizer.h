#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Node
{
	char* fileName;
	int count;
	char* token;
	struct Node* next;
}recordNode;

typedef struct HashTable
{
	recordNode** table;
	int length;
}hashTable;

typedef enum {FALSE, TRUE} boolean;

char* inputString = "this is a test sentence now^Im utilizing $%^&symbols$%^&*(*&^$as+_)(*&delimiters This  o3ne has9876568798 nu289374mbers\0";

boolean outputInitialized = FALSE;

void traverseDirectory(hashTable *myTable, const char *dir_name);

recordNode* createNode(char* fileName, char* token);

hashTable* createHashTable(int size);

recordNode* tokenize(FILE* file, char* fileName);

int validateInput(int argc);

void insertNode(recordNode* list, hashTable* hTable, char* fileName);

void printTokens(hashTable* masterTable, FILE* outputFile);

hashTable* scatterTokens(recordNode* head, int size, FILE* outputFile);

void printTokenList(hashTable* myTable, FILE* outputFile);

void printOpeningTag(FILE* outputFile);

void printClosingTag(FILE* outputFile);

void toLowerCase(recordNode* head);

int sortalnum(const char *a, const char *b);

void printHashTable(hashTable *hTable);

void deleteHashTable(hashTable *hTable);

void deleteLinkedList(recordNode* head);

void printLL(recordNode* head);

int checkOverwrite(char** argv);
