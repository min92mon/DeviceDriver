#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>

#include "minmon.h"
MODULE_LICENSE("GPL");

static int mm_major = 0, mm_minor = 0;
static int result;
static dev_t mm_dev;

static struct cdev mm_cdev;

static int mm_register_cdev(void);

static int mm_open(struct inode *inode, struct file *flip);
static int mm_release(struct inode *inode, struct file *flip);
static int mm_write(struct file *flip, const char *buf, size_t count, loff_t *f_pos);
static int mm_read(struct file *flip, const char *buf, size_t count, loff_t *f_pos);
static int mm_ioctl(struct file *flip, unsigned int cmd, unsigned long arg);

static int mm_open(struct inode *inode, struct file *flip)
{
	printk("Device Open!!!\n");

	return 0;
}
static int mm_release(struct inode *inode, struct file *flip)
{
	printk("Device Close!!!\n");
	return 0;
}
static int mm_write(struct file *flip, const char *buf, size_t count, loff_t *f_pos)
{
	char data[11];

	copy_from_user(data, buf, count);
	printk("data >>>>> = %s\n",data);
	return count;
}
static int mm_read(struct file *flip, const char *buf, size_t count, loff_t *f_pos)
{
	char data[] = "this is read func...";
	copy_to_user(buf,data,sizeof(data));
	return 0;
}
static int mm_ioctl(struct file *flip, unsigned int cmd, unsigned long arg)
{
	mm_info	ctrl_info;
	int size, err;

	if(_IOC_TYPE(cmd) != MM_MAGIC)
	{
		printk("IOC_TYPE ERROR!!!\n");
		return -EINVAL;
	}		
	if(_IOC_NR(cmd) >= MM_MAXNR)
	{
		printk("IOC_NR ERROR!!!\n");
		return -EINVAL;
	}
	size = _IOC_SIZE(cmd);

	if(size)
	{
		err = 0;
		if(_IOC_DIR(cmd) & _IOC_READ)
			err = access_ok(VERIFY_READ, (void *)arg, size);
		else if(_IOC_DIR(cmd) & _IOC_WRITE)
			err = access_ok(VERIFY_WRITE, (void *)arg, size);
		if(err)
			return err;
	}

	switch(cmd) {
		case MM_LED_OFF:
			gpio_set_value(S3C2410_GPF(2),0);
			gpio_set_value(S3C2410_GPF(3),0);
			break;	
		
		case MM_LED_ON:
			gpio_set_value(S3C2410_GPF(2),1);
			gpio_set_value(S3C2410_GPF(3),1);
			break;
		
		case MM_GETSTATE:
			printk("cmd = 2\n"); 
			printk("GPF(2):%d\n", gpio_get_value(S3C2410_GPF(2)));
			printk("GPF(3):%d\n", gpio_get_value(S3C2410_GPF(3)));
			break;

		default:
			return 0;
	}
	return 0;
}

static struct file_operations mm_fops = {
						.open = mm_open,
						.release = mm_release,
						.write = mm_write,
						.read = mm_read,
						.unlocked_ioctl = mm_ioctl,
					};

static int mm_register_cdev(void)
{
	int error;

	/* allocation device number */
	if(mm_major) {
		mm_dev = MKDEV(mm_major, mm_minor);
		error = register_chrdev_region(mm_dev, 1, "Minmon");
	} else {
		error = alloc_chrdev_region(&mm_dev, mm_minor, 1, "Minmon");
		mm_major = MAJOR(mm_dev);
	}

	if(error < 0) {
		printk(KERN_WARNING "Minmon: can't get major %d\n", mm_major);
		return result;
	}
	printk("major number=%d\n", mm_major);

	/* register chrdev */
	cdev_init(&mm_cdev, &mm_fops);
	mm_cdev.owner = THIS_MODULE;
	mm_cdev.ops = &mm_fops;
	error = cdev_add(&mm_cdev, mm_dev, 1);

	if(error)
		printk(KERN_NOTICE "Minmon Register Error %d\n", error);

	return 0;
}

static int mm_init(void)
{
    printk("Minmon Module is up... \n");
	if((result = mm_register_cdev()) < 0)
	{
		return result;
	}

	result = gpio_request(S3C2410_GPF(2),"LED 0");
	result = gpio_request(S3C2410_GPF(3),"LED 1");
	printk("complete gpio request!!\n");
	gpio_direction_input(S3C2410_GPF(2));
	gpio_direction_input(S3C2410_GPF(3));

	s3c_gpio_cfgpin(S3C2410_GPF(2),S3C2410_GPIO_OUTPUT);
	s3c_gpio_cfgpin(S3C2410_GPF(3),S3C2410_GPIO_OUTPUT);

	gpio_set_value(S3C2410_GPF(2),0);
	gpio_set_value(S3C2410_GPF(3),0);
	printk("gpio set val complete!!!\n");
    return 0;
}

static void mm_exit(void)
{
    printk("The module is down...\n");
	gpio_free(S3C2410_GPF(2));	
	gpio_free(S3C2410_GPF(3));	
	cdev_del(&mm_cdev);
	unregister_chrdev_region(mm_dev, 1);
}

module_init(mm_init); 
module_exit(mm_exit); 

