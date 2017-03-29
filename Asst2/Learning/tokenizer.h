#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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

typedef enum {FALSE, TRUE} boolean;

char* inputString = "this is a test sentence now^Im utilizing $%^&symbols$%^&*(*&^$as+_)(*&delimiters This  o3ne has9876568798 nu289374mbers\0";

boolean outputInitialized = FALSE;

void traverseDirectory(hashTable *myTable, const char *dir_name);

Node* createNode(char* file, char* token);

hashTable* createHashTable(int size);

Node* tokenize(FILE* file, char* file);

int validateInput(int argc);

void insertNode(Node* list, hashTable* hTable, char* file);

void printTokens(hashTable* masterTable, FILE* outputFile);

hashTable* scatterTokens(Node* head, int size, FILE* outputFile);

void printTokenList(hashTable* myTable, FILE* outputFile);

void printOpeningTag(FILE* outputFile);

void printClosingTag(FILE* outputFile);

void toLowerCase(Node* head);

int sortalnum(const char *a, const char *b);

void printHashTable(hashTable *hTable);

void deleteHashTable(hashTable *hTable);

void deleteLinkedList(Node* head);

void printLL(Node* head);

int exists(char** argv);
