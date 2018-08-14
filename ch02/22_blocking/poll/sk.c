#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/major.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/wait.h>


#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

#include "sk.h"
MODULE_LICENSE("GPL");

static int sk_major=0, sk_minor=0;
static int result;
static dev_t sk_dev;
static struct cdev sk_cdev;
static struct file_operations sk_fops;
static int key_input_flag;


static int sk_open(struct inode *inode, struct file *filp)
{
	s3c_gpio_cfgpin(S3C2410_GPG(4), S3C_GPIO_SFN(1));

	printk("Device has been opened\n");
	return 0;
}

static int sk_release(struct inode *inode, struct file *filp)
{
	printk("Device has been closed \n");
	return 0;
}

static int sk_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	printk("this is sk_write \n");

	char data[20];

	copy_from_user(data, buf, count);

	key_input_flag = 1;
	printk("data >>>>>> = %s \n", data);

	return count;
}

static int sk_read(struct file *filp, char *buf, size_t count, loff_t *f_ops)
{
	char data[20] = "this is read func...";
	printk(" this is sk_read \n");
	copy_to_user(buf, data, count);
	key_input_flag=0;
	return 0;
}

unsigned int sk_poll(struct file *filp, struct poll_table_struct *pt)
{
	int mask=0;
	wait_queue_head_t wq;

/*TODO: poll_wait implementation */ 

	if(key_input_flag){
		mask |= POLLIN | POLLRDNORM;
		printk(" this is sk_poll: mask is %d \n", mask);
	}
	return mask;

}

//static int sk_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg )
static int sk_ioctl(struct file *filp, unsigned int cmd, unsigned long arg )
{
	if(_IOC_TYPE(cmd)!=SK_MAGIC) return -EINVAL;
	if(_IOC_NR(cmd)>=SK_MAXNR) return -EINVAL;
	
	printk("\n sk_ioctl ");	

	switch(cmd){
		case 0: { printk("\ncmd = 0\n");break;}
		case 1: { printk("\ncmd = 1\n");break;}
		case 2: { printk("\ncmd = 2\n");break;}
		case 3: { printk("\ncmd = 3\n");break;}
		default: return 0;
	}

	return 0;
}


/*TODO: sk_poll register    */
static struct file_operations sk_fops = {
	.open	 = sk_open,
	.release = sk_release,
	.write		= sk_write,
	.read		= sk_read,
	.unlocked_ioctl		= sk_ioctl,
};

static int sk_register_cdev(void)
{
	int error;

	if(sk_major){
		sk_dev = MKDEV(sk_major, sk_minor);
		error = register_chrdev_region(sk_dev, 1, "sk");
	}else{
		error = alloc_chrdev_region(&sk_dev, sk_minor, 1 , "sk");
		sk_major = MAJOR(sk_dev);
	}

	if(error<0){
		printk(KERN_WARNING "sk: can't get major %d\n", sk_major);
		return result;
	}
	printk("major number = %d\n", sk_major);
#if 1
	/* register chrdev */
	cdev_init(&sk_cdev, &sk_fops);
	sk_cdev.owner = THIS_MODULE;
	sk_cdev.ops = &sk_fops;
	error = cdev_add(&sk_cdev, sk_dev, 1);

	if(error)
		printk(KERN_NOTICE "sk Register Error %d\n", error);
#endif 
		return 0;
}

static int __init sk_init(void)
{
	printk("The module is up... \n");

	if((result = sk_register_cdev())<0)
	{
		return result;
	}

	return 0;
}

static void __exit sk_exit(void)
{
	printk("The module is down... \n");
	cdev_del(&sk_cdev);
	unregister_chrdev_region(sk_dev, 1);
}

module_init(sk_init);
module_exit(sk_exit);
