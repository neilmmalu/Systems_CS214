#include "tokenizer.c"

int main (int argc, char** argv)
{
	hashTable* myTable = createHashTable(36);

		//FILE* inputFile = fopen(argv[2], "r");
	if(validateInput(argc) == 1)
	{
		return 1; //-1?
	}
	if (checkOverwrite(argv)==0)
	{
		return 0;
	}
	traverseDirectory(myTable, argv[2]);
	FILE* outputFile = fopen(argv[1], "w+");
	printTokens(myTable, outputFile);
	return 0;
}