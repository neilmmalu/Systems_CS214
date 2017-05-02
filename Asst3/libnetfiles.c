#include "libnetfiles.h"

typedef struct {
	char hostname[64];
	CONNECTION_MODE conn_mode;
} SERVER_CONN;

SERVER_CONN CONNECTION;

int socketGen(const char *hostname){
	int socket_fd = 0;

	struct sockaddr_in server_address;
	struct hostent *server = NULL;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd < 0){
		fprintf(stderr, "Error in libnetfiles socket() errno = %d\n", errno);
		return -1;
	}

	server = gethostbyname(hostname);
	if(server == NULL){
		errno = 0;
		h_errno = HOST_NOT_FOUND;
		printf("stderr, Host Not Found libnetfiles, h_errno");
		return -1;
	}

	bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(PORT_NO);

    if(connect(socket_fd,(struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        fprintf(stderr,"Cannot connect : libnetfiles %s, h_errno= %d\n", hostname, h_errno);
    	return -1;
    }

    return socket_fd;
}

extern int netserverinit(char *hostname, int filemode){
	
	errno =  0;		
    h_errno = 0;
	int serv_ret = 0;
    int socket_fd = -1;
    char buff[BUFF_SIZE] = "";

    strcpy(CONNECTION.hostname, buff);													
	CONNECTION.conn_mode = INVALID_FILE;

	switch(filemode){
        case UNRESTRICTED:
        case EXCLUSIVE:   
        case TRANSACTION:   
            break;

        default:
            h_errno = INVALID_FILE;
            fprintf(stderr, "bad connection mode netserverinit() \n");
            return -1;
    }

    if(hostname == NULL || strlen(hostname) == 0){
        errno = EINVAL;  
        return -1;
    }

    socket_fd = socketGen(hostname);
    if(socket_fd < 0){
        errno = 0;
       	h_errno = HOST_NOT_FOUND;
        return -1;
    }

    bzero(buff, BUFF_SIZE);
    sprintf(buff, "%d,0,0,0", 1);

    serv_ret = write(socket_fd, buff, strlen(buff));
    if(serv_ret < 0){
       	h_errno = ECOMM;  
        printf("SERVER_CONN: failed to write");
        return -1;
    }

    bzero(buff, BUFF_SIZE);
    serv_ret = read(socket_fd, buff, BUFF_SIZE - 1);  

    if(serv_ret < 0){
        h_errno = ECOMM;  
        printf("Could not read from socket\n");
        if(socket_fd != 0) 
        	close(socket_fd);
        return -1;
    }

    close(socket_fd);

    sscanf(buff, "%d,", &serv_ret);
    if(serv_ret == 0){
    	strcpy(CONNECTION.hostname, hostname);
        CONNECTION.conn_mode = (CONNECTION_MODE)filemode;
    }

    return serv_ret;
}

int netopen(const char *pathname, int flags){
	
	errno = 0;
    h_errno = 0;
	int net_addr  = -1;
    int socket_fd = -1;
    int serv_ret = 0;
    char buff[BUFF_SIZE] = "";

    if(pathname == NULL || strlen(pathname) < 1){
        printf("The pathname is invalid\n");
        errno = EINVAL;  
        return -1;
    }

    if(flags != O_RDONLY && flags != O_WRONLY && flags != O_RDWR){
		printf("Invalid flags\n");
		errno = EINVAL;
		return -1;
    }

    socket_fd = socketGen(CONNECTION.hostname);
    if(socket_fd < 0){
        errno = 0;
        h_errno = HOST_NOT_FOUND;
        fprintf(stderr, "host not found : netopen, %s\n", CONNECTION.hostname);
        return -1;
    }

    bzero(buff, BUFF_SIZE);
    sprintf(buff, "%d,%d,%d,%s", NET_OPEN, CONNECTION.conn_mode, flags, pathname);

    serv_ret = write(socket_fd, buff, strlen(buff));
    if(serv_ret < 0){
        fprintf(stderr, "failed to write cmd to CONNECTION...netopen() .  serv_ret= %d\n", serv_ret);
        return -1;
    }

	bzero(buff, BUFF_SIZE);
	serv_ret = read(socket_fd, buff, BUFF_SIZE - 1);

	if(serv_ret < 0){
        if(socket_fd != 0)
        	close(socket_fd);
        return -1;
    }

    close(socket_fd);    

    sscanf(buff, "%d,%d,%d,%d", &serv_ret, &net_addr, &errno, &h_errno);

    if(serv_ret == -1){
        printf("SERVER_CONN returns FAIL....netopen() , errno= %d (%s), h_errno=%d\n", errno, strerror(errno), h_errno);
        return -1;
    }

    return net_addr;
}

extern int netclose(int fd){
	if(fd >= 0){
        errno = EBADF;
        return -1;
    }

    errno = 0;
    h_errno = 0;

    // int net_addr  = -1;
    int socket_fd = -1;
    int serv_ret = 0;
    char buff[BUFF_SIZE] = "";


    socket_fd = socketGen(CONNECTION.hostname);
    if(socket_fd < 0){
        errno = 0;
        h_errno = HOST_NOT_FOUND;
        fprintf(stderr, "host not found...netopen(), %s\n", CONNECTION.hostname);
        return -1;
    }

    bzero(buff, BUFF_SIZE);
    sprintf(buff, "%d,%d", NET_CLOSE, fd);

    serv_ret = write(socket_fd, buff, strlen(buff));

    if(serv_ret < 0){
        h_errno = ECOMM;  
        printf("Failed write to SERVER_CONN");
        return -1;
    }

    bzero(buff, BUFF_SIZE);
    serv_ret = read(socket_fd, buff, BUFF_SIZE - 1);

    if(serv_ret < 0){
        h_errno = ECOMM;  
        printf("Failed to read from the given socket\n");
        if(socket_fd != 0)
        	close(socket_fd);
        return -1;
    }

    close(socket_fd); 
    sscanf(buff, "%d,%d,%d", &serv_ret, &errno, &h_errno);  
    return serv_ret; 
}

extern ssize_t netread(int fildes, void *buf, size_t nbyte){
	if(fildes >= 0){
        errno = EBADF;
        return -1;
    }

    errno = 0;
    h_errno = 0;

	int net_addr  = -1;
    int socket_fd = -1;
    int serv_ret = 0;
    char * temp_buffer = (char *)buf;
    char buff[BUFF_SIZE];
    

    socket_fd = socketGen(CONNECTION.hostname);
    if(socket_fd < 0){
        errno = 0;
        h_errno = HOST_NOT_FOUND;
        fprintf(stderr, "host not found...netopen(), %s\n", CONNECTION.hostname);
        return -1;
    }

	bzero(buff, BUFF_SIZE); 
	sprintf(buff, "%u,%d,%d", NET_READ, fildes, (int)nbyte);

    serv_ret = write(socket_fd, buff, strlen(buff));

    if(serv_ret < 0){
        h_errno = ECOMM;  
        printf("Failed write to SERVER_CONN");
        return -1;
    }   

	bzero(buff, BUFF_SIZE); 
	serv_ret = read(socket_fd, buff, BUFF_SIZE - 1);   

	if(serv_ret < 0){
        h_errno = ECOMM;  
        printf("Failed read from socket\n");
        if(socket_fd != 0)
        	close(socket_fd);
        return -1;
    }

    close(socket_fd); 

    sscanf(buff, "%d,%d,%d", &serv_ret, &net_addr, &h_errno);  
    strncpy(temp_buffer, buff+strlen(buff)-nbyte, nbyte);

    if(serv_ret == -1) 
    	return -1;

    return net_addr;
}

extern ssize_t netwrite(int fildes, const void *buf, size_t nbyte){

	errno = 0;
    h_errno = 0;
	int net_addr  = -1;
    int socket_fd = -1;
    int serv_ret = 0;
    char * temp_buffer = (char *)buf;
    char buff[BUFF_SIZE];
    
    if(fildes >= 0){
        errno = EBADF;
        return -1;
    }

    socket_fd = socketGen(CONNECTION.hostname);
    if(socket_fd < 0){
        errno = 0;
        h_errno = HOST_NOT_FOUND;
        fprintf(stderr, "Host was not found...netopen(), %s\n", CONNECTION.hostname);
        return -1;
    }

	bzero(buff, BUFF_SIZE); 
	sprintf(buff, "%u,%d,%d,%d,%s", NET_WRITE, fildes, (int)nbyte, (int)strlen(temp_buffer), temp_buffer);

    serv_ret = write(socket_fd, buff, strlen(buff));

    if(serv_ret < 0){
        h_errno = ECOMM;  
        printf("Failed write to SERVER_CONN");
        return -1;
    }    

	bzero(buff, BUFF_SIZE);
	serv_ret = read(socket_fd, buff, BUFF_SIZE - 1);
	if(serv_ret < 0){
        h_errno = ECOMM;  
        printf("Failed read from socket\n");
        if(socket_fd != 0)
        	close(socket_fd);
        return -1;
    }

    close(socket_fd); 

    sscanf(buff, "%d,%d,%d", &serv_ret, &net_addr, &errno);  
    if(serv_ret == -1)
    	return -1;
   
    return net_addr;     
}


