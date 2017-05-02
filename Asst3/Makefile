CC = gcc
CCFLAGS = -Wall

all : clean server

server: netfileserver.c libnetfiles.c libnetfiles.h
	gcc $(CCFLAGS) netfileserver.c -pthread -o server

clean: 
	rm -f server