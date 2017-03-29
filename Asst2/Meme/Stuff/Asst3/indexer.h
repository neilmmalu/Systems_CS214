#ifndef INDEXER_H
#include <stdlib.h>
#include"sorted-list.h"
#define INDEXER_H



struct Indexer
{
	SortedList **table;
    int size;
};
typedef struct Indexer* IndexerPtr;

typedef struct Token{
	SortedListPtr occurenceList;
	char* key;
} Token;
typedef Token* TokenPtr;

typedef struct Occurrences{
	char* fileName;
	int NumOfOcc;
}occurrence;
IndexerPtr createIndexer(int size);
void toLower(char* p);
int insertData( char* str, char* src, IndexerPtr indexer);

int compareToken(void* arg1,void* arg2);
int compareOccurrence(void* arg1,void* arg2);
void occurrenceDestroy( void * thingToDestroy);
int addOccurrenceToList(SortedListPtr list, int NumOfOcc, char* fileName);
void printOccList(SortedListPtr listPtr);
int stringCompare( void * firstArg, void * secondArg);
void stringDestroy( void * thingToDestroy);
int addStringToList(SortedListPtr list, char* str);
void printStringList(SortedListPtr listPtr);
void addTokenTolist(char* str, char* src, SortedListPtr list);

void printTokenList(SortedListPtr list);

#endif