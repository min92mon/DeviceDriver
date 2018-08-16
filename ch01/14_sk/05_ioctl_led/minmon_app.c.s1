/***************************************
 * Filename: sk_app.c
 * Title: Skeleton Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    int retn;
    char buf[100] = "write...\n";
	int flag=0;
    fd = open("/dev/Minmon",O_RDWR);

    printf("fd = %d\n", fd);
    
    if (fd<0) {
        perror("/dev/Minmon error");
        exit(-1);
    }
    else
        printf("Minmon has been detected...\n");
   
    retn = write(fd,buf,10);
    printf("\nSize of written data : %d\n", retn); 
	
    retn = read(fd,buf,20);
    printf("\ndata : %s\n",buf);

	getchar();
	ioctl(fd,0,flag);
	getchar();
	ioctl(fd,1,flag);
	getchar();
	ioctl(fd,2,flag);
	getchar();
	ioctl(fd,3,flag);
	getchar();
	ioctl(fd,4,flag);
	getchar();
	ioctl(fd,5,flag);
	getchar();
	ioctl(fd,6,flag);
	getchar();
	ioctl(fd,7,flag);
	getchar();
	ioctl(fd,8,flag);
    close(fd);
    
    return 0;
}
