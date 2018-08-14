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
#include <asm/io.h>
#include <asm/irq.h>

//#include <asm/arch/regs-gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>

#include <linux/workqueue.h>

#define DRV_NAME	"keyint"
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

// POINT: top-half ISR
static irqreturn_t mds2450_keyevent(int irq, void *dev_id, struct pt_regs *regs) {

	struct mds2450_key_detection *gd = (struct mds2450_key_detection *) dev_id;
    int state;

	state = 1;
	printk("keypad was pressed \n");

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

static int __init mds2450_keyint_init(void)
{
	int ret;

	
	s3c2410_gpio_cfgpin(S3C2410_GPF(0), S3C2410_GPF0_EINT0);
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

	printk(KERN_INFO "%s successfully loaded\n", DRV_NAME);

    return 0;
    
}



static void __exit mds2450_keyint_exit(void)
{
  
  
    free_irq(mds2450_gd.irq, &mds2450_gd);
	
#if (EXAMPLE == 100)
#elif (EXAMPLE == 200)
	//POINT: remove the tasklet
	tasklet_kill(&mds2450_gd.gdetect);
#endif

    printk(KERN_INFO "%s successfully removed\n", DRV_NAME);
}

#if 0
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
	cdev_init(&key_cdev, &rebis_keyscan_fops);
	key_cdev.owner = THIS_MODULE;
	error = cdev_add(&key_cdev, key_dev, 1);
	
	if(error)
		printk(KERN_NOTICE "Keyscan Register Error %d\n", error);
	
	return 0;
}
#endif



module_init(mds2450_keyint_init);
module_exit(mds2450_keyint_exit);

MODULE_LICENSE("GPL");

