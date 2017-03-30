CFLAGS = -ggdb -Wall

all: clean index

index: index.c tokenizer.c utils.c tokenizer.h
	gcc $(CCFLAGS) index.c -o index

clean:
	rm -rf index
