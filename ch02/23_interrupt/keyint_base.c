/*
 * Interrupt Example: request_irq
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


#define DRV_NAME "keyint"

struct mds2450_key_detection {
    int             irq;
    int             pin;
    int             pin_setting;
    char            *name;
    int             last_state;
};

static struct mds2450_key_detection mds2450_gd = {
    IRQ_EINT0, 
	S3C2410_GPF(0), 
	S3C2410_GPF0_EINT0, 
	"key-detect", 
	0
};

// TODO: ISR(Interrupt Service Routine) implementation




static int __init mds2450_keyint_init(void)
{
	int ret;
	// POINT: initialize the EINT0 
	s3c2410_gpio_cfgpin(S3C2410_GPF(0), S3C2410_GPF0_EINT0);
	// POINT: set falling Edge interrupts
	writel(readl(S3C2410_EXTINT0) & (~(0xf << 0)), S3C2410_EXTINT0);
	writel(readl(S3C2410_EXTINT0) | (0x2 << 0), S3C2410_EXTINT0); 
	
	// TODO: register the key interrupt 




	printk(KERN_INFO "%s successfully loaded\n", DRV_NAME);

    return 0;
}

static void __exit mds2450_keyint_exit(void)
{
    // TODO: free the irq


    printk(KERN_INFO "%s successfully removed\n", DRV_NAME);
}


module_init(mds2450_keyint_init);
module_exit(mds2450_keyint_exit);

MODULE_LICENSE("GPL");

