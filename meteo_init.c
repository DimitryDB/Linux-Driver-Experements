#include "meteo.h"
#include <linux/slab.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR ("DB <DimitryDB@yandex.ru>");

static int major = 0;
module_param(major,int,0644);
MODULE_PARM_DESC(major, "Major device number");

static short count = 1; 
module_param(count,short,0644);
MODULE_PARM_DESC(count, "Number of blocks");

static struct file_operations fops = {

};
// 
int __init init_module() {
    int M;
    int RetCode = 0;
    if (count < 1 || count > 256) {
        RetCode = -EINVAL;
        goto END;
    }
    AllDev = (struct meteo_group*)kmalloc(sizeof(struct meteo_group)*count,GFP_KERNEL);
    
    if (!AllDev) {
        printk( KERN_ALERT "Out of memory (**meteo_item)\n" );
        RetCode = -ENOMEM;
        goto END;
    }
    
    memset(AllDev,0, count* sizeof(struct meteo_group));
    AllCount = count;
    
    M = register_chrdev(major, METEO_NAME, &fops);
    if (M < 0 ) {
        printk( KERN_ALERT "Error: %d\n", M);
        RetCode = M;
        goto END1;
    } else if (major == 0) {
        major = M;
    }
    printk( KERN_ALERT "'%s' registered with number %d\n", METEO_NAME, major);
    return 0;

//    END2:
//    unregister_chrdev(major, METEO_NAME);

END1:
    kfree(AllDev);
    
END:
    return RetCode;
}

 void __exit cleanup_module() {
    kfree(AllDev);
    unregister_chrdev(major, METEO_NAME);
    printk( KERN_ALERT "'%s' unregistered\n", METEO_NAME);
}
