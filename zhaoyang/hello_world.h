
#ifndef ZHAOYANG_HEAD_FILE
#define ZHAOYANG_HEAD_FILE

#include <linux/module.h>  
#include <linux/kernel.h> 
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>	// struct attribute
#include <linux/device.h>  // struct device_attribute
#include <linux/types.h> // ssize_t
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/fs.h> // alloc_chrdev_region and unregister_chrdev_region,file_operations
#include <linux/kdev_t.h> // dev_t
#include <linux/cdev.h> // cdev
#include <linux/slab.h> // kmalloc kfree
#include <asm/uaccess.h> // copy_to_user and copy_from_user

#define DEVICES_ZHAOYANG_NAME "zhaoyang"

#define printk_zhaoyang(fmt,arg...) printk("zhaoyang@@ " fmt,##arg)
#define printk_zhaoyang_error(fmt,arg...) printk("zhaoyang@@-error " fmt,##arg)
#define printk_function_in printk("zhaoyang(^.^) Now we are in %s\n",__FUNCTION__)


#endif