/*
*	G2450_ADC.C - The s3c2450 adc module.
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <asm/delay.h>
#include <linux/io.h>
#include <plat/adc.h>
#include <plat/devs.h>
#include <linux/platform_device.h>
#include <mach/regs-gpio.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/input.h>

#define INPUT_TACT_0	11
#define INPUT_TACT_1	12

#define INPUT_SCAN_0	1
#define INPUT_SCAN_1	2
#define INPUT_SCAN_2	3
#define INPUT_SCAN_3	4
#define INPUT_SCAN_4	5
#define INPUT_SCAN_5	6
#define INPUT_SCAN_6	7
#define INPUT_SCAN_7	8
#define INPUT_SCAN_8	9
#define INPUT_SCAN_9	10	

#define DRV_NAME	"key_input"
#define S3C2450_INPUT_BT_VER	1
static struct input_dev *input_dev;

static void kscan_timer_handler(unsigned long data); 

#define MDS2450_KSCAN_MAJOR 67
static char kscan_name[] = "mds2450-kscan";

#define KSCAN_TIME	(1*HZ / 5)
static struct timer_list kscan_timer = TIMER_INITIALIZER(kscan_timer_handler, 0, 0);


static int key_scan( int row )
{
	int ret = 0;
	int key;

	if( 0 == row )
	{	// 1 ~ 5
		gpio_set_value(S3C2410_GPG(0), 1);
		gpio_set_value(S3C2410_GPF(7), 0);
	}
	else
	{	// 6 ~ 10
		gpio_set_value(S3C2410_GPG(0), 0);
		gpio_set_value(S3C2410_GPF(7), 1);
	}

	if( 0 ==  gpio_get_value(S3C2410_GPF(2)))
	{
		key = INPUT_SCAN_0 + (5 * row);

		input_report_key( input_dev, key, 1 ); 
		input_sync( input_dev );

		input_report_key( input_dev, key, 0 ); 
		input_sync( input_dev );

		ret = 1;
	}
	
	if( 0 ==  gpio_get_value(S3C2410_GPF(3)))
	{
		key = INPUT_SCAN_1 + (5 * row);

		input_report_key( input_dev, key, 1 ); 
		input_sync( input_dev );

		input_report_key( input_dev, key, 0 ); 
		input_sync( input_dev );

		ret = 1;
	}
	
	if( 0 ==  gpio_get_value(S3C2410_GPF(4)))
	{
		key = INPUT_SCAN_2 + (5 * row);

		input_report_key( input_dev, key, 1 ); 
		input_sync( input_dev );

		input_report_key( input_dev, key, 0 ); 
		input_sync( input_dev );

		ret = 1;
	}
	
	if( 0 ==  gpio_get_value(S3C2410_GPF(5)))
	{
		key = INPUT_SCAN_3 + (5 * row);

		input_report_key( input_dev, key, 1 ); 
		input_sync( input_dev );

		input_report_key( input_dev, key, 0 ); 
		input_sync( input_dev );

		ret = 1;
	}

	if( 0 ==  gpio_get_value(S3C2410_GPF(6)))
	{
		key = INPUT_SCAN_4 + (5 * row);

		input_report_key( input_dev, key, 1 ); 
		input_sync( input_dev );

		input_report_key( input_dev, key, 0 ); 
		input_sync( input_dev );

		ret = 1;
	}

	return ret;
}


static int key_tact ( void )
{
	int ret = 0;

	// EINT0
	if( 0 ==  gpio_get_value(S3C2410_GPF(0)))
	{
		input_report_key( input_dev, INPUT_TACT_0, 1 ); 
		input_sync( input_dev );

		input_report_key( input_dev, INPUT_TACT_0, 0 ); 
		input_sync( input_dev );

		ret = 1;
	}

	// EINT1
	if( 0 ==  gpio_get_value(S3C2410_GPF(1)))
	{
		input_report_key( input_dev, INPUT_TACT_1, 1 ); 
		input_sync( input_dev );

		input_report_key( input_dev, INPUT_TACT_1, 0 ); 
		input_sync( input_dev );

		ret = 1;
	}

	return ret;
}


static void kscan_timer_handler(unsigned long data)
{
	int lp;
	int ret;

	for( lp=0 ; lp<2 ; lp++ )
	{
		ret = key_scan(lp);
	}

	mod_timer(&kscan_timer, jiffies + (KSCAN_TIME));
}


static irqreturn_t mds2450_kscan_irq(int irq, void *dev_id, struct pt_regs *regs)
{
	int ret;

	//ret = key_tact();
	printk("key pressed\n");
	return IRQ_HANDLED;
}


//static int __devinit mds2450_kscan_probe(struct platform_device *pdev)
static int __init mds2450_kscan_init(void)
{
//	int lp;
	int ret = 0;

	printk(KERN_INFO "ready to scan key value\n");

	// GPIO Initial
	s3c_gpio_cfgpin(S3C2410_GPF(0), S3C_GPIO_SFN(2));		// EINT0
/*
	s3c_gpio_cfgpin(S3C2410_GPF(1), S3C_GPIO_SFN(2));
	s3c_gpio_setpull(S3C2410_GPF(0), S3C_GPIO_PULL_NONE);
	s3c_gpio_setpull(S3C2410_GPF(1), S3C_GPIO_PULL_NONE);

	s3c_gpio_cfgpin(S3C2410_GPF(2), S3C_GPIO_SFN(0));
	s3c_gpio_cfgpin(S3C2410_GPF(3), S3C_GPIO_SFN(0));
	s3c_gpio_cfgpin(S3C2410_GPF(4), S3C_GPIO_SFN(0));
	s3c_gpio_cfgpin(S3C2410_GPF(5), S3C_GPIO_SFN(0));
	s3c_gpio_cfgpin(S3C2410_GPF(6), S3C_GPIO_SFN(0));		// 
	s3c_gpio_setpull(S3C2410_GPF(2), S3C_GPIO_PULL_NONE);
	s3c_gpio_setpull(S3C2410_GPF(3), S3C_GPIO_PULL_NONE);
	s3c_gpio_setpull(S3C2410_GPF(4), S3C_GPIO_PULL_NONE);
	s3c_gpio_setpull(S3C2410_GPF(5), S3C_GPIO_PULL_NONE);
	s3c_gpio_setpull(S3C2410_GPF(6), S3C_GPIO_PULL_NONE);

	s3c_gpio_cfgpin(S3C2410_GPF(7), S3C_GPIO_SFN(1));
	s3c_gpio_cfgpin(S3C2410_GPG(0), S3C_GPIO_SFN(1));
*/
	// Request IRQ
//	if( request_irq(IRQ_EINT0, mds2450_kscan_irq, IRQF_DISABLED | IRQF_TRIGGER_FALLING , "MDS2450_KEY_0", NULL ) )     
	if( request_irq(IRQ_EINT0, (void *)mds2450_kscan_irq, IRQF_DISABLED | IRQF_TRIGGER_FALLING , DRV_NAME, NULL ) )     
	{
		printk("failed to request external interrupt. - EINT0\n");
		ret = -ENOENT;
		return ret;
	}
	printk(KERN_INFO "%s successfully loaded\n", DRV_NAME);
/*
	if( request_irq(IRQ_EINT1, mds2450_kscan_irq, IRQF_DISABLED | IRQF_TRIGGER_FALLING , "MDS2450_KEY_1", NULL ) )     
	{
		printk("failed to request external interrupt. - EINT1\n");
		ret = -ENOENT;
		return ret;
	}
*/
/*
	// Scan timer
	mod_timer(&kscan_timer, jiffies + (KSCAN_TIME));

	// input device register
	input_dev = input_allocate_device();
	if(!input_dev){
		printk("Fail Input Device Alloc!\n");
		ret = -ENOMEM;
		goto fail;
	}

	input_dev->evbit[0] 	= BIT(EV_KEY);

	input_dev->name    	 	= DRV_NAME;
	input_dev->id.bustype   = BUS_HOST;
	input_dev->id.vendor    = 0xDEAD;
	input_dev->id.product   = 0xBEEF;
	input_dev->id.version   = S3C2450_INPUT_BT_VER;

	for( lp=0 ; lp<14 ; lp++ )
		set_bit( lp, input_dev->keybit );

	// Register Input Device
	input_register_device( input_dev );
*/
	return 0;

fail:
	free_irq( IRQ_EINT0, NULL );
//	free_irq( IRQ_EINT1, NULL );

	return ret;

}

//static int __devexit mds2450_kscan_remove(struct platform_device *pdev)
static int __exit mds2450_kscan_exit(struct platform_device *pdev)
{
	printk(KERN_INFO "end of the scanning\n");

	free_irq( IRQ_EINT0, NULL );
	free_irq( IRQ_EINT1, NULL );

	del_timer_sync(&kscan_timer);


	return 0;
}
/*
static struct platform_driver mds2450_kscan_device_driver = {
	.probe      = mds2450_kscan_probe,
	.remove     = __devexit_p(mds2450_kscan_remove),
	.driver     = {
		.name   = "mds2450-kscan",
		.owner  = THIS_MODULE,
	}
};
*/
/*
static int __init mds2450_kscan_init(void)
{

 	return platform_driver_register(&mds2450_kscan_device_driver);
}

static void __exit mds2450_kscan_exit(void)
{
	platform_driver_unregister(&mds2450_kscan_device_driver);
}
*/
module_init(mds2450_kscan_init);
module_exit(mds2450_kscan_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("GIT <gemini525@nate.com>");
MODULE_DESCRIPTION("Key driver for MDS2450");
