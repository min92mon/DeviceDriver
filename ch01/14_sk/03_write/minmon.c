/***************************************
 * Filename: sk.c
 * Title: Skeleton Device
 * Desc: register and unregister chrdev
 ***************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");

static int mm_major = 0, mm_minor = 0;
static int result;
static dev_t mm_dev;

static struct cdev mm_cdev;

static int mm_register_cdev(void);

static int mm_open(struct inode *inode, struct file *flip);
static int mm_release(struct inode *inode, struct file *flip);
static int mm_write(struct file *flip, const char *buf, size_t count, loff_t *f_pos);

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
static struct file_operations mm_fops = {
						.open = mm_open,
						.release = mm_release,
						.write = mm_write,
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

    return 0;
}

static void mm_exit(void)
{
    printk("The module is down...\n");
    /* TODO: delete cdev object*/
	cdev_del(&mm_cdev);
	unregister_chrdev_region(mm_dev, 1);
}

module_init(mm_init); 
module_exit(mm_exit); 

