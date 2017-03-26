#ifndef tokenizer_H
#include <stdlib.h>
#define tokenizer_H
struct TokenizerT_ {
	char* originalString;
	char* currentString;
};

typedef struct TokenizerT_ TokenizerT;

typedef struct tokenType {
	char* string;
	char* type;
} token;

TokenizerT *TKCreate( char * ts );
token *TTCreate(char* str, const char* type);
void removeCharAt(char** string, int index);
char * substring(char** stringPtr, int startIndex, int endIndex);
void TKDestroy( TokenizerT * tk );
void TDestroy( token * toke );
int deleteFirstWhiteSpace(TokenizerT * tk);
int sizeOfChecker(const char* str);
token *wordCheck(TokenizerT *tk);
char *TKGetNextToken( TokenizerT * tk );

int main(int argc, char** argv);
#endif