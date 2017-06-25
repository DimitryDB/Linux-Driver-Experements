#include <linux/kthread.h>
#include <asm/uaccess.h>

#include "meteo.h"

// #include <linux/slab.h>
// 
// #include "cdev.h"
// 

#define MIN  MINOR(file->f_inode->i_rdev)

int meteo_item_dispatcher (void *data) {
    struct meteo_item *Dt = (struct meteo_item*)data;
    if (!Dt) return -ENOENT;
    
    Dt->f_status = METEO_ACTIVE;
    if(!try_module_get (THIS_MODULE)) {
        printk(KERN_ALERT "Cannot lock module\n");
        Dt->f_status = METEO_INACTIVE;
        return -EACCES;
    }
    // Work
    while (!kthread_should_stop()) {
        // Check if possible to read
        printk(KERN_ALERT "Step Counted\n");
        schedule_timeout_interruptible(1000);
    }
    
    Dt->f_status = METEO_INACTIVE;
    module_put(THIS_MODULE);
    return 0;
    
}

 ssize_t meteo_write(struct file *file, const char __user *buffer, size_t length, loff_t *ofs) { 
     int ItemNo=0;
     int ItemCount =0;
     int k;
     
     if (length == 0) return 0; 

     if(*ofs <0 ) {
         printk(KERN_ALERT "Negative offset\n");
         return -EIO;
     }
     if (length % sizeof(struct meteo_item) > 0) {
         printk(KERN_ALERT "Invalid length\n");
         return -EINVAL;
     }
     
     ItemCount = length/sizeof(struct meteo_item);
     
     if (*ofs % sizeof(struct meteo_item) > 0) {
        printk(KERN_ALERT "Invalid offset\n");
        return -EIO;
     }
     ItemNo =*ofs / sizeof(struct meteo_item);
     
     if (ItemNo+ItemCount > METEO_COUNT) {
        printk(KERN_ALERT "Invalid item number\n");
        return -EIO;
     }


    for (k=0; k < ItemCount; k++) {
        
        copy_from_user(&(AllDev[MIN].Buffer),buffer+k*sizeof(struct meteo_item),sizeof(struct meteo_item));
        if (AllDev[MIN].Items[ItemNo+k].f_status == AllDev[MIN].Buffer.f_status) continue;
        if (AllDev[MIN].Buffer.f_status == METEO_ACTIVE ) {
            AllDev[MIN].Threads[ItemNo+k] = kthread_run( meteo_item_dispatcher, (void*)(AllDev[MIN].Items+ItemNo+k),
                                                  "meteo%dn%d", (int)MIN,ItemNo+k);
            // check for errors
        } else {
            kthread_stop(AllDev[MIN].Threads[ItemNo+k]);
            AllDev[MIN].Threads[ItemNo+k] = 0;
        //  stop thread
        //  whait untill thread stoped    
        }
    }
    
    printk(KERN_ALERT "%lu bytes written\n", length);
    return length;
}

// loff_t cdev_llseek(struct file *file, loff_t ofs, int whence) {
//     switch (whence) {
//         case SEEK_CUR :
//             file->f_pos+=ofs;
//             break;
//         case SEEK_SET :
//             file->f_pos=ofs;
//             break;
//         case SEEK_END :
//             file->f_pos=DevContSize+ofs;
//             break;
//         default:
//             return -EINVAL;
//     }
//     return 0;
// }
