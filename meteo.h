 
#ifndef METEO_H
#define METEO_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>

#define METEO_NAME "meteo"

#define METEO_COUNT 16

// double representation sizeof(double)
#define DATASIZE 8 

struct meteo_item {
    char    f_ID[4];
    int     f_status;
    char    f_data[DATASIZE];  // double representation
    time_t  f_last;
};
 
#define METEO_ACTIVE 1
#define METEO_INACTIVE 0

struct meteo_group {
    struct meteo_item Buffer;
    struct task_struct *Threads[METEO_COUNT];
    struct meteo_item Items[METEO_COUNT];
};

extern struct meteo_group   *AllDev;
extern size_t                AllCount;


int meteo_open (struct inode *inode, struct file *file);

int meteo_release (struct inode *inode, struct file *file );

ssize_t meteo_read(struct file *file, char __user *buffer, size_t length, loff_t *ofs);

loff_t meteo_llseek(struct file *file, loff_t ofs, int whence) ;

ssize_t meteo_write(struct file *file, const char __user *buffer, size_t length, loff_t *ofs); 

#endif //METEO_H
