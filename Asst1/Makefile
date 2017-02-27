CC  = gcc
//CCFLAGS = -Wall -lm -Werror

all : memgrind  

memgrind : memgrind.o
	$(CC) $(CCFLAGS) -o memgrind memgrind.o

memgrind.o : memgrind.c
	$(CC) $(CCFLAGS) -c memgrind.c

clean:
	rm -f memgrind memgrind.o