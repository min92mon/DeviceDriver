#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/slab.h>
#include <linux/vmalloc.h>

void kmalloc_test( void )
{
    char *buff;
	unsigned long allocSize;    
    printk( "kmalloc test\n" );
    allocSize = 1024;
    buff = kmalloc(allocSize,GFP_KERNEL);
    if( buff != NULL )
    {
        sprintf( buff, "test memory: %lu\n", allocSize);
        printk( buff );
    
        kfree( buff );
    }    
    // allocSize = PAGE_SIZE * 1000; 
	allocSize = KMALLOC_MAX_SIZE;
    buff = kmalloc(allocSize,GFP_KERNEL);
    if( buff != NULL )
    {
        printk( "Big Memory Ok:%lu\n", allocSize);
        kfree( buff );
    }
    
}

void vmalloc_test( void )
{
    char *buff;
	unsigned long allocSize;
    printk( "vmalloc test\n" );
    allocSize = KMALLOC_MAX_SIZE + 100;
    buff = vmalloc(allocSize);
    if( buff != NULL )
    {
        sprintf( buff, "vmalloc test ok:%lu\n", allocSize );
        printk( buff );
    
        vfree( buff );
    }    

}

void get_free_pages_test( void )
{
    char *buff;
    int   order;

    printk( "get_free_pages test\n" );
    
    order = get_order(8192*10);
    buff  = (char *)(__get_free_pages( GFP_KERNEL, order ));
    if( buff != NULL) 
    {
        sprintf( buff, "__get_free_pages test ok [%d]\n", order );
        printk( buff );
        
        free_pages((unsigned long)buff, order);
    }
}

int memtest_init(void)      
{ 
    //char *data;
    
    printk("Module Memory Test\n" );
    
    kmalloc_test();
    vmalloc_test();
    get_free_pages_test();
    
    return 0; 
}

void memtest_exit(void)  
{ 
    printk("Module Memory Test End\n"); 
}

module_init(memtest_init);
module_exit(memtest_exit);

MODULE_LICENSE("Dual BSD/GPL");
