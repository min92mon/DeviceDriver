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
    close(fd);
    
    return 0;
}
