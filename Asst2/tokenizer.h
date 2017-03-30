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

/*
*Inserts newly created nodes into the hashtable after sorting the tokens provided. Adds the file *the tokens were found in.
*/
void insertNode(char* fileName, Node* head, hashTable* list);

/*
*Traverses recursively through the directory specified and touches all files. Takes unique tokens, *calls the tokenize function and adds them to a node. It then inserts the node into the main *hashtable. Handles errors and closes the directory.
*/
void traverseDirectory(const char* directoryName, hashTable* list);

/*
*Creates the node with the file and tokens corresponding to them
*/
Node* createNode(char* fileName, char* token);

/*
*Creates the main hashtable which stores the tokens, and then outputs them
*/
hashTable* createHashTable(int size);

/*
*Goes through the file specified and tokenizes the tokens. Stores the tokens in a stream *temporarily which is then stored in the corresponding node
*/
Node* tokenize(FILE* file, char* fileName);

/*
*Prints the opening XML tags and corresponding file name tags and word text tags
*/
void printTokens(hashTable* masterTable, FILE* outputFile);

/*
*Calls the create hashtable method and calls the print tokens method to store the nodes in the *hashtable and then print the tags
*/
void addTokens(int size, FILE* outputFile, Node* head);

/*
*Called in the main method and ultimately uses other helper methods to print the output in the *specified output file by going through the main hashtable
*/
void outputTokenList(hashTable* myTable, FILE* outputFile);

/*
*Helps sort alphanumeric file names by checking if digits are present, checking the file length and returning either positive, negative, or 0
*/
int sortHelper(const char* a, const char* b);

void deleteTable(hashTable* list);

int exists(char** argv);

