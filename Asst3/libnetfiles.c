#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "libnetfiles.h"

typedef struct {
	char hostname[64];
	CONNECTION_MODE fMode;
} SERVER_CONN;

SERVER_CONN clientConn;

int getSocket(const char *hostname){
	int sockfd = 0;

	struct sockaddr_in serv_addr;
	struct hostent *server = NULL;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		fprintf(stderr, "libnetfiles: socket() failed errno = %d\n", errno);
		return -1;
	}

	server = gethostbyname(hostname);
	if(server == NULL){
		errno = 0;
		h_errno = HOST_NOT_FOUND;
		printf("stderr, libnetfiles: host not found, h_errno");
		return -1;
	}

	
}