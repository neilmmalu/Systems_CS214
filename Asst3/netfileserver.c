#include "libnetfiles.h"

static int check = 0;

file_data data_table[DATA_TABLE_SIZE];



int main(int argc, char *argv[]){
    int socket_fd = 0;
    int newsocket_fd = 0;
    pthread_t thread_id = 0;

    struct sockaddr_in server_address, client_address;
    int client_address_length = sizeof(client_address);
    createTable();
 	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        fprintf(stderr,"netfileserver: socket() failed, errno= %d\n", errno);
        exit(EXIT_FAILURE);
    }

	memset((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT_NO);
    if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        fprintf(stderr,"netfileserver: bind() failed, errno= %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd, 50) < 0){
        fprintf(stderr,"netfileserver: listen() failed, errno= %d\n", errno);
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    // else{}


while(check == 0){
        printf("netfileserver: listener is waiting to accept incoming request\n");
        if ((newsocket_fd = accept(socket_fd, (struct sockaddr *)&client_address, &client_address_length)) < 0){
            if (errno != EINTR){
                fprintf(stderr,"netfileserver: accept() failed, errno= %d\n", errno);
            	close(newsocket_fd);
            	if(socket_fd != 0)
            		close(socket_fd);
                exit(EXIT_FAILURE);
            }
            else{
            	close(newsocket_fd);
            	if(socket_fd != 0)
            		close(socket_fd);
                check = 1;  
            }
        }
        else{
            printf("netfileserver: listener accepted a new request from socket\n");
            pthread_create(&thread_id, NULL, &thread_process, &newsocket_fd );
            printf("netfileserver: listener spawned a new worker thread with ID Thread : %lu\n",thread_id);
        }
    }

    if(newsocket_fd != 0)
    	close(newsocket_fd);
    if(socket_fd != 0 )
    	close(socket_fd);

    printf("netfileserver: terminated\n");
    return 0;
}

void *thread_process(void *socket){
    int *socket_fd = socket;
    file_data *newFd = NULL;
    int wbuff_length = 0;
    int temp = 0;
    int file_descriptor = -1;
    char buff[BUFF_SIZE] = "";
    NET_FUNCTION_TYPE function_type = INVALID;

    temp = pthread_detach(pthread_self());
    temp = read(*socket_fd, buff, BUFF_SIZE -1);

    if ( temp < 0 ){
        fprintf(stderr,"Thread: %lu failed to read from socket\n", pthread_self());
        if(*socket_fd != 0)
        	close(*socket_fd);
		pthread_exit(NULL);
    }
    else{
        printf("Thread: %lu received \"%s\"\n", pthread_self(), buff);
    }

    int * nbyte = malloc(sizeof(int));
    char readBuffer [500];
    sscanf(buff, "%u,", &function_type);
   
    switch(function_type){

        case NET_SERVERINIT:
            sprintf(buff, "%d,0,0,0", 0);
            printf("Thread : %lu responding with \"%s\"\n", pthread_self(), buff);
            break;

        case NET_OPEN:
            newFd = malloc(sizeof(file_data));

            sscanf(buff, "%u,%d,%d,%s", &function_type, (int *)&(newFd->conn_mode), &(newFd->flag), newFd->file_pwd);

            temp = open(newFd);
            if (temp == -1){
				memset(buff, BUFF_SIZE);
                sprintf(buff, "%d,%d,%d,%d", -1, temp, errno, h_errno);
            } 
            else {
				memset(buff, BUFF_SIZE);
                sprintf(buff, "%d,%d,%d,%d", 0, temp, errno, h_errno);
            }
            printf("Thread : %lu responding with \"%s\"\n", pthread_self(), buff);
            free(newFd);
            break;

        case NET_READ:
            sscanf(buff, "%u,%d, %d", &function_type, &file_descriptor, nbyte);
            temp = read(file_descriptor, *nbyte, readBuffer);
            if(temp==-1){
                sprintf(buff, "%d,%d,%d,%d", -1, errno, h_errno, temp);
            }
            else{
                readBuffer[temp]='\0';
                sprintf(buff, "%d,%d,%d,%s", 0, temp, errno, readBuffer);
            }
	    	free(nbyte);
            break;

        case NET_WRITE: 
            sscanf(buff, "%u,%d,%d,%d", &function_type, &file_descriptor, nbyte, &wbuff_length);
	    	strncpy(readBuffer, buff+strlen(buff)-wbuff_length, *nbyte);
            temp = write(file_descriptor, readBuffer, *nbyte);
            if(temp == -1){
                sprintf(buff, "%d,%d,%d", -1, errno, h_errno);
                printf("write Failed with %d\n", temp);
            }
            else{
                sprintf(buff, "%d,%d,%d", 0, temp, errno);
                printf("write succeeded with %d\n", temp);
            }
	    	free(nbyte);
            break;

        case NET_CLOSE:
            sscanf(buff, "%u,%d", &function_type, &file_descriptor);
            temp = deleteFD(file_descriptor);                               
            if(temp == -1){
                sprintf(buff, "%d,%d,%d,%d", -1, errno, h_errno, temp);
            } 
            else{
                sprintf(buff, "%d,%d,%d,%d", 0, errno, h_errno, temp);
            }
            break;

        case INVALID:
            break;

        default:
            printf("Thread : %lu received invalid net function\n", pthread_self());
            break;

	}    
	temp = write(*socket_fd, buff, (strlen(buff)+1));                                                // Send Server response back to client
    //printf("value of n: %d\n", n);
    if(temp < 0){
        fprintf(stderr,"Thread : %lu fails to write to socket\n", pthread_self());
    }
    
    pthread_exit(NULL);
}


int open(file_data *new){
    int res = -1;
    if(canOpen(new) == 0){
        return -1;
    }
    new->local_fd = open(new->file_pwd, new->flag);
    if(new->local_fd < 0)
    	return -1;
    res = createFD(new);
    if(res == -1){
        errno = ENFILE;
        return -1;
    } 
    return res;  
}


int canOpen(file_data *new){
    int i;
    for(i=0; i < DATA_TABLE_SIZE; i++){

        if (strcmp(data_table[i].file_pwd, new->file_pwd) == 0){
            if(data_table[i].conn_mode == TRANSACTION || new->conn_mode == TRANSACTION){
				return 0;
	    	}

	    	if(data_table[i].conn_mode == UNRESTRICTED && (data_table[i].flag == O_WRONLY || data_table[i].flag == O_RDWR)){
				if(new->conn_mode == EXCLUSIVE && (new->flag == O_WRONLY || new->flag == O_RDWR)){
					return 0;
				}
	    	}

	    	if(data_table[i].conn_mode == EXCLUSIVE && (data_table[i].flag == O_WRONLY || data_table[i].flag == O_RDWR)){
				if(new->conn_mode == UNRESTRICTED && (new->flag == O_WRONLY || new->flag == O_RDWR)){
					return 0;
				}

				if(new->conn_mode == EXCLUSIVE && (new->flag == O_WRONLY || new->flag == O_RDWR)){
					return 0;
				}
	    	}
        }
    }
    return 1;
}


void createTable(){
    int i = 0;
    for (i=0; i < DATA_TABLE_SIZE; i++) {
        data_table[i].local_fd = 0;
        data_table[i].server_fd = 0; 
        data_table[i].conn_mode = INVALID_FILE;        
        data_table[i].flag = O_RDONLY;        
        data_table[i].file_pwd[0] = '\0';        
    }
}


int createFD(file_data *new){
    int i = 0;
    int n = -1;

    for (i=0; i < DATA_TABLE_SIZE; i++) {
        if (data_table[i].local_fd == 0 ){
            data_table[i].local_fd = new->local_fd;
            data_table[i].server_fd = (-10 * (i + 5));
            data_table[i].conn_mode = new->conn_mode;        
            data_table[i].flag = new->flag;        
            strcpy(data_table[i].file_pwd, new->file_pwd);
            return data_table[i].server_fd;  
         }
    }
    return -1;                                                                           
}

int deleteFD(int server_fd){
	int n = close(data_table[(server_fd/-10) - 5].local_fd);
    if(n < 0)
    	return -1;

    int i=(server_fd/ -10) - 5;
    if(i < DATA_TABLE_SIZE && i >= 0){
        data_table[i].local_fd = 0;
        data_table[i].server_fd = 0;  
        data_table[i].conn_mode = INVALID_FILE;        
        data_table[i].flag = O_RDONLY;        
        data_table[i].file_pwd[0] = '\0';     
        return 0;
    }
    
    errno = EBADF;
    return -1;
}

int read(int file_descriptor, ssize_t nbyte, char *readBuffer){
    int n = -1;
    int i=(file_descriptor/ -10) - 5;
   //printf("Makes it here netread\n");
    //printf("LocalFD: %d\n", data_table[i].local_fd);
    if(i<DATA_TABLE_SIZE && i>=0){
        if(data_table[i].flag == O_WRONLY){
            errno = EBADF;
            return -1;
        }
        n = read(data_table[i].local_fd, readBuffer, (ssize_t)nbyte);
        printf("return of read %d\n", n);
        if(n >= 0){
        	return n;
        	printf("Makes it past read\n");
        }
    }
    
    errno = EBADF;
    return -1;

}

int write(int file_descriptor, char * readBuffer, ssize_t nbyte){
    int n = -1;
	int i=(file_descriptor/ -10) - 5;
    if(i < DATA_TABLE_SIZE && i >= 0){
        if(data_table[i].flag==O_RDONLY){
            errno = EBADF;
            return -1;
        }
        n = write(data_table[i].local_fd, readBuffer,(ssize_t)nbyte);
        printf("return of write%d\n", n);
        if(n >= 0){
        	return n;
        	printf("Makes it past read\n");
        }
    }
    
    errno = EBADF;
    return -1;
}




