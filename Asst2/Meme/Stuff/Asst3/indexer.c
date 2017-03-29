#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include"indexer.h"

typedef struct Indexer Indexer;
int debugging = 1;
void debug(char* str){
	if(debugging){
		printf("%s\n",str);
	}
}

int compareToken(void* arg1,void* arg2){
	if(!arg1 && !arg2){
		return 0;
	}
	else if(!arg1){
		return -1;
	}
	else if(!arg2){
		return 1;
	}
	
	TokenPtr t1 = (TokenPtr)arg1;
	TokenPtr t2 = (TokenPtr)arg2;
	
	return stringCompare(t1->key,t2->key);
}

void tokenDestroy( void * thingToDestroy)
{
  TokenPtr toDestroy = (TokenPtr) thingToDestroy;
  if(toDestroy!=NULL){
    //Should we free the string? I'm leaning towards no
    SLDestroy(toDestroy->occurenceList);
    free( toDestroy->key );
    free( toDestroy );
  }

	return;
}
TokenPtr createToken(char* str, char* src){
	char* toFile = malloc(sizeof(char)*(strlen(src)+2));
	strcpy(toFile,src);
	TokenPtr token = (TokenPtr)malloc(sizeof(Token));
	token->key = str;
	occurrence * occPtr = (occurrence*)malloc(sizeof(occurrence));
	occPtr->NumOfOcc=1;
	occPtr->fileName=toFile;

	token->occurenceList = SLCreate(compareOccurrence,occurrenceDestroy);
	SLInsert(token->occurenceList, (void*)occPtr);
	return token;
}

void printVoidOccurrence(void * ghost){
	if(!ghost){
		printf("Got NULL Ghost\n");
		return;
	}
	printf("\t%s : %d\n",((occurrence *)ghost)->fileName , ((occurrence *)ghost)->NumOfOcc);
}

void printVoidToken(void * ghost){
	if(!ghost){
		debug("Got NULL Ghost\n");
		return;
	}
	printf("%s\n",((TokenPtr)ghost)->key);
	SortedListPtr list = (SortedListPtr)((TokenPtr)ghost)->occurenceList;
	SortedListIteratorPtr iIter = SLCreateIterator(list);
	do{
		printVoidOccurrence(SLGetItem(iIter));
	}while(SLNextItem(iIter));
}

void printTokenList(SortedListPtr list){
	SortedListIteratorPtr iIter = SLCreateIterator(list);
	do{
		printVoidToken(SLGetItem(iIter));
	}while(SLNextItem(iIter));
}

int compareOccurrence(void* arg1,void* arg2){
	if(!arg1 && !arg2){
		return 0;
	}
	else if(!arg1){
		return -1;
	}
	else if(!arg2){
		return 1;
	}

	if(((occurrence*)arg1)->NumOfOcc == ((occurrence*)arg2)->NumOfOcc){
		return stringCompare(((occurrence*)arg1)->fileName,((occurrence*)arg2)->fileName);
	}
	else if(((occurrence*)arg1)->NumOfOcc > ((occurrence*)arg2)->NumOfOcc){
		return 1;
	}
	else{
		return -1;
	}

}
void occurrenceDestroy( void * thingToDestroy)
{
  occurrence * toDestroy = (occurrence*) thingToDestroy;
  // printf("Got to int Destroy to destroy: %d\n",*intToDestroy);
  if(toDestroy!=NULL){
    free(toDestroy->fileName);
    free( toDestroy );
  }

	return;
}
int addOccurrenceToList(SortedListPtr list, int NumOfOcc, char* fileName){
	char* toFile = malloc(sizeof(char)*(strlen(fileName)+2));
	strcpy(toFile,fileName);
	if(!list){
		return 0;
	}
	occurrence * occPtr = (occurrence*)malloc(sizeof(occurrence));
	occPtr->NumOfOcc=NumOfOcc;
	occPtr->fileName=toFile;
	return SLInsert(list, (void*)occPtr);
}


void printOccList(SortedListPtr listPtr){
	if(listPtr==NULL){
		printf("Null List\n");
		return;
	}
	Node* ptr = listPtr->head;
	if(ptr==NULL){
		printf("Empty List\n");
		return;
	}
	while(ptr){
			printf("%s\t",((occurrence *)ptr->data)->fileName);
			ptr=ptr->next;
	}
	printf("\n");
}

int stringCompare( void * firstArg, void * secondArg)
{
	char * firstString = (char*)firstArg;
	char * secondString= (char*)secondArg;
	int c = strcmp(firstString,secondString);
	if( c < 0){
		return 1;
	}

	else if( c > 0){
		return -1;
	}

	return 0;
}

void stringDestroy( void * thingToDestroy)
{
  char * stringToDestroy = (char*)thingToDestroy;
  //printf("Got to int Destroy to destroy: %d\n",*intToDestroy);
  if(stringToDestroy!=NULL){
    free( stringToDestroy );
  }

  return;
}
int addStringToList(SortedListPtr list, char* str){
	if(!list){
		return 0;
	}
	char* stringPtr = (char*)malloc(strlen(str)*sizeof(char));
	strcpy(stringPtr,str);
	return SLInsert(list, (void*)stringPtr);
}
void printStringList(SortedListPtr listPtr){
	if(listPtr==NULL){
		printf("Null List\n");
		return;
	}
	Node* ptr = listPtr->head;
	if(ptr==NULL){
		printf("Empty List\n");
		return;
	}
	while(ptr){
			printf("%s\t",(char *)ptr->data);
			ptr=ptr->next;
	}
	printf("\n");
}

IndexerPtr createIndexer(int size){
	IndexerPtr newIndexer;
	if(size<1){
		debug("Size too small");
		return NULL;
	}
	if( (newIndexer = (IndexerPtr) malloc(sizeof(Indexer))) == NULL){
		return NULL;
	}
	if( (newIndexer->table = (SortedListPtr *) malloc(sizeof(SortedListPtr) * size)) == NULL){
		return NULL;
	}
	int i;
	for(i=0; i<size; i++){
		newIndexer->table[i] = NULL;
	}
	newIndexer->size = size;

	return newIndexer;
}

void toLower(char* str){
	int i;
	for(i =0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
}

// void addTokenTolist(char* str, char* src, SortedListPtr list){
	
// }
SortedListPtr wrapToken(TokenPtr token){
	SortedListPtr list = SLCreate(compareToken,tokenDestroy);
	SLInsert(list, (void*)token);
	return list;
}

int insertData( char* str, char* src, IndexerPtr indexer){
	if(!indexer || !(indexer->table) || !src || !str || str[0]=='\0' || !isalpha(str[0])){
		debug("Bad Data");
		return 0;
	}

	toLower(str);
	
	int hash = (str[0] - 'a') % indexer->size;

	if(!indexer->table[hash]){
		TokenPtr token = createToken(str,src);
		indexer->table[hash] = wrapToken(token);
		return 1;
	}
	else{
		TokenPtr token;
		SortedListPtr list = indexer->table[hash];
		SortedListIteratorPtr tokenIter = SLCreateIterator(list);
		
		do{
			token = SLGetItem(tokenIter);
			if(token && strcmp(token->key,str)==0){
				SortedListIteratorPtr occurIter = SLCreateIterator(token->occurenceList);
				if(!occurIter){
					occurrence * occPtr = (occurrence*)malloc(sizeof(occurrence));
					occPtr->NumOfOcc=1;
					char* toFile = malloc(sizeof(char)*(strlen(src)+2));
					strcpy(toFile,src);
					occPtr->fileName=toFile;
					debug("Got here!");
					SLInsert(token->occurenceList, (void*)occPtr);

					SLDestroyIterator(tokenIter);
					return 1;
				}
				do{
					occurrence* occ = (occurrence*)SLGetItem(occurIter);
					if(occ && strcmp(occ->fileName,src)==0){
						occurrence * occPtr = (occurrence*)malloc(sizeof(occurrence));
						occPtr->NumOfOcc = occ->NumOfOcc+1;
						char* toFile = malloc(sizeof(char)*(strlen(src)+2));
						strcpy(toFile,src);
						occPtr->fileName = toFile;
						
						SLRemove(token->occurenceList, (void*)occ);
						SLInsert(token->occurenceList, (void*)occPtr);

						SLDestroyIterator(tokenIter);
						return 2;
					}
				}while(SLNextItem(occurIter));
				occurrence * occPtr = (occurrence*)malloc(sizeof(occurrence));
				occPtr->NumOfOcc = 1;
				char* toFile = malloc(sizeof(char)*(strlen(src)+2));
				strcpy(toFile,src);
				occPtr->fileName = toFile;
				SLInsert(token->occurenceList, (void*)occPtr);

				SLDestroyIterator(tokenIter);
				return 1;
			}
		}while(SLNextItem(tokenIter));
		token = createToken(str,src);
		SLInsert(indexer->table[hash],token);

		SLDestroyIterator(tokenIter);
		return 1;
	}

	return 0;
}
