#ifndef _MM_H_
#define _MM_H_

#define	MM_MAGIC	'k'
#define MM_MAXNR	6

typedef struct 
{
	unsigned long size;
	unsigned char buff[128];
} __attribute__((packed)) mm_info;

#define MM_LED_OFF	_IO(MM_MAGIC, 0)
#define MM_LED_ON	_IO(MM_MAGIC, 1)
#define MM_GETSTATE	_IO(MM_MAGIC, 2)
#define MM_READ		_IOR(MM_MAGIC, 3, mm_info)
#define MM_WRITE	_IOW(MM_MAGIC, 4, mm_info)
#define MM_RW		_IOWR(MM_MAGIC, 5, mm_info)

#endif
