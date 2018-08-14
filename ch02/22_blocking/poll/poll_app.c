#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termio.h>
#include <linux/poll.h>


int main(void)
{
        int fd, retn;
		int pid;
        int flag = 0;
        char key;
        struct  pollfd Events[2];

	    char buf[20] = "this is fucntion..\n";
		
		fd=open("/dev/SK", O_RDWR);
//		sleep(2);
//			ioctl(fd, 1, flag);
//		write(fd, buf, 20);
	
		pid=fork();
		if(pid<0){
			printf("error\n");
			exit(-1);
		}
		else if(pid==0){
			sleep(3);
			write(fd, buf, 20);

			sleep(5);
			write(fd, buf, 20);

            printf("[APP]Child process_write!!\n");
			exit(0);
		}
		else {

       	while(1){
	    Events[0].fd            = fd;
        Events[0].events        = POLLIN; //waiting read
    	retn = poll(Events, 1, 5000);    // Event waiting
	
        if(retn < 0)
        {
               perror("Poll_Test");
               exit(0);
        }
        if(retn == 0) {
            printf("[APP]Wakeup_Poll_Event:No Event!!\n");
			continue;
        } 

		else {
		if(Events[0].revents & POLLIN){
                        read( fd, &key, 1 );
                        printf("[APP]Wakeup_Poll_Event!!\n");
                        printf("Key : %d\n", key );
						
//			            close(fd);
//						exit(0);
			
                }

        	}
		}
		}
		close(fd);
        return 0;
}

