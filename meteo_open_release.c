 
#include "meteo.h"

#define MIN MINOR(inode->i_rdev)

 int meteo_open (struct inode *inode, struct file *file) {
    int RetCode=0;
    if (MIN >= AllCount) {
        RetCode = -ENOENT;
        goto END;
    }
    if(!try_module_get (THIS_MODULE)) {
        printk(KERN_ALERT "Cannot lock module\n");
        RetCode = -EACCES;
        goto END;
    }
    // Check if device allready open  
    printk(KERN_ALERT "Device opened\n");
    return 0;
//  END2:
    //cleanUp
//  END1:
    //module_put(THIS_MODULE);
END:
    return RetCode;
}

int meteo_release (struct inode *inode, struct file *file ) {
    // Set cleanUp parameters
    module_put(THIS_MODULE);
    printk(KERN_ALERT "Device Closed\n");
    return 0;
}
