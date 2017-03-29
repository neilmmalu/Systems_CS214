#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"tokenizer.h"



TokenizerT *TKCreate( char * ts ) {
  //check that ts is not NULL
	if(ts==NULL){
		return NULL;
	}
	//Create a TokenizerT struct that holds the "original string" and the string we are currently working on
	TokenizerT* tokenizer =  malloc(sizeof(TokenizerT));
	tokenizer->originalString= malloc(sizeof(char)*(strlen(ts)+1));
	//copy input string into orginal string to preserve it
	strcpy(tokenizer->originalString,ts);
	tokenizer->originalString[strlen(tokenizer->originalString)] = '\0';
	tokenizer->currentString= malloc(sizeof(char)*(strlen(ts)+1));
	//copy input into current string in order to keep track of what we are currently tokenizing
	strcpy(tokenizer->currentString,ts);
	tokenizer->currentString[strlen(tokenizer->currentString)] = '\0';
   
	return tokenizer;
}
//seperate structure that holds individual tokens and their types
token *TTCreate(char* str, const char* type){
  if(str==NULL || type == NULL){
    return NULL;
  }
  token *toke =malloc(sizeof(token));
  toke->string = malloc(sizeof(char)*(strlen(str)+1));
  strcpy(toke->string,str);
  toke->string[strlen(toke->string)] = '\0';
  toke->type= malloc(sizeof(char)*(strlen(type)+1));
  strcpy(toke->type,type);
  toke->type[strlen(toke->type)] = '\0';
  return toke;

}


//removes the character at the specified index of the string
void removeCharAt(char** string, int index){
	if(index>=strlen(*string)){
		// checks if index is greater than length of string
		return;
	}
	memmove(&(*string)[index], &(*string)[index + 1], strlen(*string) - index);
}

// used to seperate a token that has been found at specified index and returns the token that was chopped off
char * substring(char** stringPtr, int startIndex, int endIndex){
	char * string=*stringPtr;
	char* sub= malloc(sizeof(char)*(endIndex-startIndex+2));
	int i=0;
	for(i=0;i<(endIndex-startIndex+1);i++){
		sub[i]=string[startIndex];
		removeCharAt(&string,startIndex);
	}
	sub[(endIndex-startIndex+1)]='\0';
	return sub;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
//used to free TokenizerT structs
void TKDestroy( TokenizerT * tk ) {
	free(tk->originalString);
	free(tk->currentString);
	free(tk);
}
//used to free token structs
void TDestroy( token * toke ) {
	free(toke->string);
	free(toke->type);
	free(toke);
}
int deleteFirstWhiteSpace(TokenizerT * tk){
	
	//switch statement takes in the char at the beginning of the index and decides if it is a delimiter if so, removes it
	if(!isalpha(tk->currentString[0])){
		removeCharAt(&(tk->currentString),0);
		return 0;
	}
		return -1;
	
}
// compares inputed string to "sizeof(" returns 0 if there is a match
int sizeOfChecker(const char* str){
  return strcmp("sizeof(",str);
}
      
//function that creates a "word" token, filled with alphanumerical chars
token *wordCheck(TokenizerT *tk){
  token *tt = NULL;
  //to account for the "sizeof()" opperator
  if(tk->currentString[0] == 's'){
    char *str = malloc(sizeof(char)*8);
    strncpy(str, tk->currentString,7);
    str[7] = '\0';
    int check = sizeOfChecker(str);
    free(str);
    if(check == 0){
      int index = 7;
      while(tk->currentString[index] != ')'){
	index++;
      }
      tt = TTCreate(substring(&(tk->currentString),0,index), "sizeof()");
      return tt;
    }
  }
  int index = 1;
  int endIndex; 
  //iterates through the string until a char that is non-alphanumeric is found
  while(isalnum(tk->currentString[index])){
    //to account for the "sizeof()" opperator found inside of a current alphnumeric sequence
    if(tk->currentString[index] == 's'){
      if((tk->currentString[index+1]== 'i') && (tk->currentString[index+2]== 'z') && (tk->currentString[index+3]== 'e') && (tk->currentString[index+4]== 'o') && (tk->currentString[index+5]== 'f') && (tk->currentString[index+6]== '(')){
	  break;
      }
      
      
    }
    index++;
  }
  endIndex = index-1;
  const char* word = "word";
  //creates a token struct using the alphanumeric substring as the string and "word" as the type
  tt = TTCreate(substring(&(tk->currentString),0,endIndex), word);
  return tt;
  
}
char *TKGetNextToken( TokenizerT * tk ) {

	if(tk==NULL || tk->currentString==NULL || strlen(tk->currentString)<1 || (tk->currentString)[0]=='\0'){
		return NULL;
	}
	token *tt = NULL;
	//takes care of delimiters
	int del = deleteFirstWhiteSpace(tk);
	if(del >= 0){
	  tt = TTCreate(substring(&(tk->currentString),0,(del-1)), "delim");
	  char *str = tt->string;
	  TDestroy(tt);
	  return str;
	}
	if(strlen(tk->currentString)<1 || (tk->currentString)[0] == '\0' ){
		return NULL;
	}
	//store first char and see what kind of token we are dealing with
	char ch = tk->currentString[0];
	//is the token a word?
	if(isalpha(ch)){
		tt = wordCheck(tk);
	}
	
	else{
		tt = NULL;
	}

	if(tt==NULL){
		return NULL;
	}
	//store string to return before free struct
	char* str = tt->string;
	free(tt->type);
	free(tt);
	//print the token and type
	// printf("%s ",tt->type);
	// printf("\"%s\"\n",tt->string);
	//TDestroy(tt);
	return str;
}
