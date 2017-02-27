CC = gcc
CCFLAGS = -Wall

all: memgrind

memgrind: memgrind.c mymalloc.c mymalloc.h
	$(CC) $(CCFLAGS) -g -o memgrind memgrind.c

clean:
	rm memgrind