#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "libnetfiles.h"


int main(){
	
	char buffer[5000];
	int netfd1;
	int netfd2;
	int bytesread;
	int byteswritten;
	
	int a = netserverinit("localhost", UNRESTRICTED);
	printf("netserverinit returns: %d\n", a);
	
	netfd1 = netopen("file1.txt", O_RDONLY);
	printf("netopen returns: %d\n", netfd1);
	netfd2 = netopen("file2.txt", O_WRONLY);
	printf("netopen returns: %d\n", netfd2);
	
	bytesread = netread(netfd1, buffer, 120);
	printf("netread returns: %d\n", bytesread);
	printf("buffer: %s\n", buffer);

	byteswritten = netwrite(netfd2, buffer, 70);
	printf("netwrite returns: %d\n", byteswritten);
	
	a = netclose(netfd1);
	printf("netclose returns: %d\n", a);
	a = netclose(netfd2);
	printf("netclose returns: %d\n", a);

	return 0;
}
