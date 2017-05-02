CC = gcc
CCFLAGS = -Wall

all : libnetfiles netfileserver clean

libnetfiles: libnetfiles.c libnetfiles.h tester.c
	$(CC) $(CCFLAGS) -o client libnetfiles.c tester.c

netfileserver: netfileserver.c libnetfiles.c libnetfiles.h
	$(CC) $(CCFLAGS) netfileserver.c -pthread -o server

clean: 
	rm -f server client