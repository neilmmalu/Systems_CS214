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
	traverseDirectory(argv[2], mainTable);
	FILE* mainOutputFile = fopen(argv[1], "w");
	fprintf(mainOutputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(mainOutputFile, "<fileIndex>\n");
	outputTokens(mainTable, mainOutputFile);
	deleteTable(mainTable);
	return 0;
}