/*
 * hellomisc.c
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/miscdevice.h>


// module attributes
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Youngdeok");
MODULE_DESCRIPTION("Hello misc Module");

static char msg[100] = {0};
static short readPos = 0;
static int times = 0;

// prototypes 
static int dev_open(struct inode *, struct file *);
static int dev_close(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

// structure containing callbacks
static struct file_operations fops = 
{
	.read = dev_read,
	.open = dev_open,
	.write = dev_write,
	.release = dev_close,
};

// TODO: define hello_miscdev using miscdevice structure minor number : 253, name: hellomisc, file_operations: fops
struct miscdevice hello_miscdev = {
	253, "hellomisc", &fops
};


// called when modules is loaded
static int hello_init(void)
{
	int ret = 0;
	
	// TODO: register the hello_miscdev device
	ret = misc_register( &hello_miscdev );
	if ( ret<0 ) {
		printk(KERN_ERR "Hello Misc Driver register error\n");
		goto err;
	}

	printk(KERN_ALERT "Hello! Misc Driver~\n");

	return 0;
err:
	return ret;
}

// called when module in unloaded 
static void hello_exit(void)
{
	// TODO: unregister the hello_miscdev device
	misc_deregister( &hello_miscdev );
	printk(KERN_ALERT "Good Bye~ Misc Driver!\n");
}

// called when open system call is done on the device file
static int dev_open(struct inode *inod, struct file *filp)
{
	times++;
	printk(KERN_ALERT "Misc Device opened %d times\n", times);
	return 0;
}

// called when read system call is done on the device file
static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
	short count = 0;
	while (len && (msg[readPos] != 0 ))
	{
		// copy byte from kernel space to user space
		put_user(msg[readPos], buff++);
		count++;
		len--;
		readPos++;
	}
	return count;
}

// called when write system call is done on the device file
static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	short ind = len - 1;
	short count = 0;
	memset(msg, 0, 100);
	readPos = 0;
	while ( len>0 ) {
		// copy the given string to the driver but in reverse
		msg[count++] = buff[ind--];
		len--;
	}
	return count;
}

// called when close system call is done on the device file
static int dev_close(struct inode *inod, struct file *filp)
{
	printk(KERN_ALERT "Device Closed\n");
	return 0;
}


module_init(hello_init);
module_exit(hello_exit);

