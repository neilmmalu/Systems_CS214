CC = gcc
CCFLAGS = -Wall

all : netfileserver

netfileserver: netfileserver.c libnetfiles.c libnetfiles.h
	$(CC) $(CCFLAGS) netfileserver.c -pthread -o server

clean: 
	rm -f server
