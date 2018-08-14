/*
 * Interrupt Example: Bottom-half(workqueue, tasklet)
 */
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/irq.h>

//#include <asm/arch/regs-gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>

#include <linux/workqueue.h>

#include <linux/cdev.h> //cdev_init
#include <linux/wait.h> //wait_event_interruptible
#include <asm/uaccess.h> //user access
#include <linux/fs.h> //file_operatios 

#include <linux/platform_device.h> //platform_driver_register()

#define DRV_NAME	"keyint"

static int key_major = 0, key_minor = 0;
static int result;
struct cdev key_cdev;
static dev_t key_dev = 0;
static int flag = 0;

static DECLARE_WAIT_QUEUE_HEAD(wq);

/*TODO: ldd_device structure*/



// POINT: change to the bottom technique
#define EXAMPLE 200 
//======================================
// 100 : workqueue test
// 200 : tasklet test
// others : non bottom-half
//======================================

#if 1
	#define gprintk(fmt, x... ) printk( "%s: " fmt, __FUNCTION__ , ## x)
#else
	#define gprintk(x...) do { } while (0)
#endif



struct mds2450_key_detection {
    int irq;
    int pin;
    int pin_setting;
    char *name;
    int  last_state;
#if (EXAMPLE == 100)
	// TODO: define a work_struct object
	struct work_struct gdetect;
#elif (EXAMPLE == 200)
	// TODO: define a tasklet_struct object
	struct tasklet_struct gdetect;
#endif
};

static struct mds2450_key_detection mds2450_gd = {
    IRQ_EINT0, 
	S3C2410_GPF(0), 
	S3C2410_GPF0_EINT0, 
	"key-detect", 
	0
};

static int key_register_cdev(void);

// POINT: top-half ISR
static irqreturn_t mds2450_keyevent(int irq, void *dev_id, struct pt_regs *regs) {

	struct mds2450_key_detection *gd = (struct mds2450_key_detection *) dev_id;
    int state;
	unsigned long flags;

	local_irq_save(flags);
	local_irq_disable();

	printk("\nkeypad was pressed \n");

	local_irq_restore(flags);
	state = 1;

  if (!gd)
      return IRQ_HANDLED;

#if 1
    state = s3c2410_gpio_getpin(gd->pin);
    gd->last_state = state;
    gprintk("%s gd %s\n\n", gd->name, state ? "high" : "low");
#endif


#if (EXAMPLE == 100)
	// TODO: set the schedule work
	schedule_work(&gd->gdetect);
#elif (EXAMPLE == 200)
	// TODO: set the tasklet schedule
	tasklet_schedule(&gd->gdetect);
#endif

	flag = 1;
	wake_up_interruptible(&wq);

	return IRQ_HANDLED;

}

#if (EXAMPLE == 100)
// POINT: workqueue callback handler
static void mds2450_keyint_callback(void *pgd)
{
	gprintk("workqueue callback call\n\n");
}

#elif (EXAMPLE == 200)
// POINT: tasklet callback handler
static void mds2450_keyint_callback(ulong data)
{
	gprintk("tasklet callback call\n\n");
}
#endif

static ssize_t mds2450_keyint_read(struct file *filp, char *buff, size_t count, loff_t *offp)
{
	int i;

	if(!(filp->f_flags & O_NONBLOCK)){
		#if 0
		interruptible_sleep_on(&wq);
		#else
		wait_event_interruptible(wq, flag != 0);
		//wait_event_interruptible_timeout(wq, flag != 0, 600);
		#endif
	}	

	return count;
}


static int mds2450_keyint_open(struct inode * inode, struct file * file)
{

	printk(KERN_INFO "ready to scan key value\n");

	return 0;
}

static int mds2450_keyint_release(struct inode * inode, struct file * file)
{
	printk(KERN_INFO "end of the scanning\n");

	return 0;
}

static struct file_operations mds2450_keyint_fops = {
	.owner		= THIS_MODULE,
	.open		= mds2450_keyint_open,
	.release	= mds2450_keyint_release,
	.read		= mds2450_keyint_read,
};


/***TODO: bus registeration ****/
///*

#if 0
static int ldd_match(struct device *dev, struct device_driver *driver)
{
	return !strncmp(dev->init_name, driver->name, strlen(driver->name));
}
#endif

/*TODO: ldd_bus_type structure */





/*TODO: ldd_bus structure */


/*TODO: ldd_device structure */



/*TODO: device_driver(s3c2410keypad_driver) structure */



/*TODO: key_class_release fn */



/*TODO: key_class structure */


/*TODO: key_cdevice_release fn */


/*TODO: key_cdevice structure */



/*TODO: ldd_dev_release fn */


/*TODO: ldd_device_register/unregister fn*/



static int __init mds2450_keyint_init(void)
{
	int ret;
	key_cdevice.devt = key_dev;

	key_register_cdev();

	s3c2410_gpio_cfgpin(S3C2410_GPF(0), S3C2410_GPF0_EINT0);
	s3c_gpio_setpull(S3C2410_GPF(0), S3C_GPIO_PULL_NONE);
	// falling Edge interrupt
	writel(readl(S3C2410_EXTINT0) & (~(0xf << 0)), S3C2410_EXTINT0);
	writel(readl(S3C2410_EXTINT0) | (0x2 << 0), S3C2410_EXTINT0); 

	// to register the key interrupt
	
	if( request_irq( IRQ_EINT0, (void *)mds2450_keyevent, IRQF_DISABLED, DRV_NAME, &mds2450_gd) )   
    {
        printk("failed to request external interrupt.\n");
        ret = -ENOENT;
        return ret;
    }

#if (EXAMPLE == 100)
	// TODO: initialize the workqueue callback handler
	INIT_WORK(&mds2450_gd.gdetect, (void *)mds2450_keyint_callback); 

#elif (EXAMPLE == 200)
	// TODO: initialize the tasklet handler
	tasklet_init(&mds2450_gd.gdetect, 
			mds2450_keyint_callback, (unsigned long)(&mds2450_gd)); 
#endif
	
		/*** bus register ***/



	printk(KERN_INFO "%s successfully loaded\n", DRV_NAME);
  	
	return 0;
    
}

static void __exit mds2450_keyint_exit(void)
{
  
 	cdev_del(&key_cdev);
	unregister_chrdev_region(key_dev, 1);  	
  
  free_irq(mds2450_gd.irq, &mds2450_gd);
	
#if (EXAMPLE == 100)
#elif (EXAMPLE == 200)
	//POINT: remove the tasklet
	tasklet_kill(&mds2450_gd.gdetect);
#endif

		/*** bus unregister ***/


    printk(KERN_INFO "%s successfully removed\n", DRV_NAME);
}


static int key_register_cdev(void)
{
    int error;
	
	/* allocation device number */
	if(key_major) {
		key_dev = MKDEV(key_major, key_minor);
		error = register_chrdev_region(key_dev, 1, dev_name);
	} else {
		error = alloc_chrdev_region(&key_dev, key_minor, 1, dev_name);
		key_major = MAJOR(key_dev);
	}
	
	if(error < 0) {
		printk(KERN_WARNING "keyscan: can't get major %d\n", key_major);
		return result;
	}
	printk("major number=%d\n", key_major);
	
	/* register chrdev */
	cdev_init(&key_cdev, &mds2450_keyint_fops);
	key_cdev.owner = THIS_MODULE;
	error = cdev_add(&key_cdev, key_dev, 1);
	
	if(error) {
		printk(KERN_NOTICE "Keyscan Register Error %d\n", error);
	  return 0;
	}
}

module_init(mds2450_keyint_init);
module_exit(mds2450_keyint_exit);

MODULE_LICENSE("GPL");

