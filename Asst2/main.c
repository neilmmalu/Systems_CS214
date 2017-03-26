#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<dirent.h>
#include"tokenizer.h"
#include"indexer.h"
#include"sorted-list.h"

int deBugging = 1;
void debugger(char* str){
	if(deBugging){
		printf("%s\n",str);
	}
}

char* getFileName(char* filepath){
	int x;
	int k;
	int checker = 0;
	for(x = 0; x < strlen(filepath); x++){
		if(filepath[x] == '/'){
			k = x;
			checker = 1;
		}
	} 
	if(checker == 0){
		return filepath;
	}
	else{
	char* str = malloc(sizeof(char)*strlen(filepath)+2);
	k++;
	int y;
	for(y = 0; filepath[k];y++){
		str[y] = filepath[k];
		k++;
	}
	str[y] = '\0';
	return str;
	}
}
void printToFile(char* outputFile, IndexerPtr indexer){
	FILE* outputFilePtr = fopen(outputFile,"w");
	if(outputFilePtr == NULL){
		return;
	}
	fprintf(outputFilePtr, "%c%s%c%c\n", '{', "\"list\"", ':', '[');

	int i;
	int count = 0;
	int count2 = 0;
	for(i=0;i<indexer->size;i++){
		if(indexer->table[i]){
			count++;
		}
	}
	for(i=0;i<indexer->size;i++){
		if(indexer->table[i]){
			count2++;
			SortedListIteratorPtr iter = SLCreateIterator(indexer->table[i]);
			TokenPtr tk = SLGetItem(iter);
			while(tk){
				fprintf(outputFilePtr, "\t{\"%s\" : [\n", tk->key);
				SortedListIteratorPtr iter2 = SLCreateIterator(tk->occurenceList);
				occurrence* occ;
				while((occ = SLGetItem(iter2)) != NULL){
					occurrence* tempOcc = occ;

					occ = SLNextItem(iter2);

					if(occ){
						fprintf(outputFilePtr, "\t\t{\"%s\" : %d%s\n", tempOcc->fileName,tempOcc->NumOfOcc,"},");
					}
					else{
						fprintf(outputFilePtr, "\t\t{\"%s\" : %d%s\n", tempOcc->fileName,tempOcc->NumOfOcc,"}");
					}
				}
				tk = SLNextItem(iter);
				if(tk || count2 < count){
					fprintf(outputFilePtr, "\t]%s\n","},");	
				}
				else{
					fprintf(outputFilePtr, "\t]%s\n","}");
				}
				SLDestroyIterator(iter2);
			}
			SLDestroyIterator(iter);
		}
	}
	fprintf(outputFilePtr, "]}");
	fclose(outputFilePtr);
}

void indexTest(){
	IndexerPtr indexer = createIndexer(26);

	char arr[] = "Hello";
	char* str = &arr[0];
	char arr2[] = "Nowhere";
	char* str2 = &arr2[0];

	if(!indexer || !(indexer->table) || !str || str[0]=='\0' || !isalpha(str[0])){
		debugger("Bad Test Data");
	}
	insertData(str,str2,indexer);
	insertData(str,str2,indexer);
	insertData(str,str2,indexer);

	str = &arr[1];
	insertData(str,str2,indexer);

	char arr3[] = "averywhere";
	char* str3 = &arr3[0];
	insertData(str,str3,indexer);


	int i;
	for(i=0;i<indexer->size;i++){
		if(indexer->table[i]){
			printTokenList(indexer->table[i]);
		}
	}
}

void readFile(char *fileName, IndexerPtr indexer)
{
	FILE *pfile;
	pfile = fopen(fileName, "r");

	if(pfile == NULL){
		printf("No file found\n");
		return;
	}

	//get size of file
	fseek(pfile, 0, SEEK_END);
	long fSize = ftell(pfile);
	rewind(pfile);

	//read entire file into string
	char* string = (char*)malloc(fSize * sizeof(char));
	fread(string, sizeof(char), fSize, pfile);
	
	fclose(pfile);

	//tokenize string	
	TokenizerT *tokenizer = TKCreate(string);
	char* token;

	if(tokenizer != NULL)
	{
		while((token = TKGetNextToken(tokenizer)) != NULL)
		{
		
			int i;
			for(i =0; i<strlen(token); i++){
				token[i] = tolower(token[i]);
			}
			if(isalpha(token[0])){

				char* str = getFileName(fileName);
				insertData(token,str,indexer);
			}
		}
		TKDestroy(tokenizer);
	}
	free(string);
}

int directoryHelper(char* directory, IndexerPtr indexer){
	DIR* dir = opendir(directory);
	if(dir == NULL){
		// not a directory
		return 0;
	}
	struct dirent *pDirent;
	
	while((pDirent = readdir(dir)) != NULL)
	{
		char c[strlen(pDirent->d_name)+1];
		strcpy(c, pDirent->d_name);
		char newStr[(strlen(directory)+1)+(strlen(c)+1)+1];
		strcpy(newStr, directory);
		strcat(newStr,"/");
		strcat(newStr,c);

		if(strcmp(c,".") != 0 && strcmp(c,"..") != 0 && c[0] != '.')
		{	
			if(directoryHelper(newStr, indexer) != 1)
			{
				readFile(newStr, indexer);
			}
			
		} 		
	}
	closedir(dir);
	return 1;
}


int main(int argc, char** argv){
	if(argc != 3){
		printf("Not enough information\n");
		return 0;
	}
	IndexerPtr indexer = createIndexer(26);
	int check = directoryHelper(argv[2], indexer);
	if(check == 0){
	readFile(argv[2],indexer);
	}
	printToFile(argv[1], indexer);
	int i;
	for(i=0;i<indexer->size;i++){
		if(indexer->table[i]){
			SLDestroy(indexer->table[i]);
		}
	}
	free(indexer->table);
	free(indexer);
	// indexTest();
	return 0;
}