#include "tokenizer.c"

int main(int argc, char** argv)
{
	hashTable* mainTable = createHashTable(36);

	if(argc!=3)
    {
	  	printf("usage: ./index output_file target_file/directory \n");
        return 1;
    }
	if (exists(argv) == 0)
	{
		return 0;
	}
	traverseDirectory(mainTable, argv[2]);
	FILE* mainOutputFile = fopen(argv[1], "w+");
	outputTokens(mainTable, mainOutputFile);
	deleteTable(mainTable);
	return 0;
}