CFLAGS = -ggdb -Wall

all: clean index

index: index.c tokenizer.c navigation.c tokenizer.h
	gcc $(CCFLAGS) index.c -o index

clean:
	rm -rf index
