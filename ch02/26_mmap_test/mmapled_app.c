#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 

#include <sys/mman.h> 

// gpn 15 led control
#define GPIO_BASE_ADDR		0x56000000  //-- GPIO BASE ADDRESS
#define GPIO_SIZE		    0x00000100	//-- ���� �Ҵ� �޸��� Size
#define OFFSET_GPGCON		0x60
#define OFFSET_GPGDAT		0x64
#define OFFSET_GPGPUD		0x68



void led_test (void *addr)
{
	int i=0; 
	
	unsigned int *pgpgcon;
	unsigned int *pgpgdat;
	
	pgpgcon = (unsigned int *)(addr + OFFSET_GPGCON);
	pgpgdat = (unsigned int *)(addr + OFFSET_GPGDAT);
	
	*pgpgcon = ((*pgpgcon & ~(0x03<<10))| (0x01<<10)); // outmode setting
	for( i=0; i<10; i++)
	{
		*pgpgdat = (*pgpgdat|(1<<5)); // led off
		sleep(1);
		*pgpgdat = (*pgpgdat & ~(1<<5)); // led on
		sleep(1);
	}
}


int main(void)
{ 
	int fd;
	void *addr;

	fd = open( "/dev/mem", O_RDWR|O_SYNC ); 
	if( fd < 0 ) 
	{ 
		perror( "/dev/mem open error" ); 
		exit(1); 
	} 

	// IO �޸𸮸� ��´�. 
	addr = mmap( 0, 				                //-- Ŀ�ο��� �˾Ƽ� �Ҵ��û 
				GPIO_SIZE, 							//-- �Ҵ� ũ�� 
				PROT_READ|PROT_WRITE, MAP_SHARED, 	//-- �Ҵ� �Ӽ� 
				fd, 								//-- ���� �ڵ� 
				GPIO_BASE_ADDR ); 					//-- ���� ����� �����ּ� 

	if( addr == NULL )
	{
		printf("mmap fail ==> exit()");
		exit(1);
	}

	printf( "fd value %d\n", fd );
	printf( "IO ADDR %p\n", addr ); //-- ���� ��(Virtual) Base Address
	
	led_test( addr );
	
	
	munmap(addr, GPIO_SIZE ); // ���εȰ� ���� 
	close (fd);
	

	return 0; 
}
