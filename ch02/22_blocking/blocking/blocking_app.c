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
    int retn;
    int fdp, fdc;
	int pid, i;

    char buf[100] = "this is fucntion..\n";
 
/*TODO: open sleepy device file */   

    if (fdp < 0) {
        perror("/dev/sleepy open error...!!");
        exit(-1);
    }
    //else
    //    printf("Sleepy has been detected...(parent)\n");a
    
/* fork  */


	if(pid < 0) {
		perror("fork error!!!");
		exit(-1);
	}
	else if(pid == 0) {	// child
/*TODO: open sleepy device file */  


/*TODO: read systemcall */  


	}
	else {				// parent

/*TODO: write systemcall */  



	}

}
