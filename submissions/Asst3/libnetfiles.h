#ifndef LIBNETFILES_H
#define LIBNETFILES_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/stat.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 512
#define DATA_TABLE_SIZE 512
#define PORT_NO 32768

typedef enum {
    UNRESTRICTED = 0,
    EXCLUSIVE   = 1,
    TRANSACTION = 2,
    INVALID_FILE = 99
} CONNECTION_MODE;

typedef enum {
    NET_SERVERINIT = 1,
    NET_OPEN  = 2,
    NET_READ  = 3,
    NET_WRITE = 4,
    NET_CLOSE = 5,
    INVALID   = 99
} NET_FUNCTION_TYPE;

typedef struct{
    int  server_fd;
    int local_fd;
    CONNECTION_MODE conn_mode;
    int flag;
    char file_pwd[256];
} file_data;

extern int netserverinit(char *hostname, int filemode);
extern int netopen(const char *pathname, int flags);
extern ssize_t netread(int fildes, void *buf, size_t nbyte); 
extern ssize_t netwrite(int fildes, const void *buf, size_t nbyte); 
extern int netclose(int fd);
int socketGen(const char *hostname);

void createTable();
int createFD(file_data *new);
int deleteFD(int server_fd);
int canOpen(file_data *new);
void *thread_process(void *socket);
int local_open(file_data *new);
int local_read(int file_descriptor, ssize_t nbyte, char * readBuffer);
int local_write(int file_descriptor, char * readBuffer, ssize_t nbyte);

#endif    // _LIBNETFILES_H_