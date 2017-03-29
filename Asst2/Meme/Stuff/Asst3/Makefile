all: main.c libi.a 
	gcc -o index main.c libi.a

libi.a: indexer.o indexer.h tokenizer.o tokenizer.h sorted-list.o sorted-list.h
	ar r libi.a indexer.o tokenizer.o sorted-list.o

indexer.o: indexer.c indexer.h
	gcc -c -Wall indexer.c

tokenizer.o: tokenizer.c tokenizer.h
	gcc -c -Wall tokenizer.c

sorted-list.o: sorted-list.c sorted-list.h
	gcc -c -Wall sorted-list.c

clean:
	rm *.o;	rm ./index; rm libi.a