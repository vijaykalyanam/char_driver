#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include "prototypes.h"

MODULE_LICENSE("GPL"); 

int cdriver_alloc(struct cdev **cdev) {

	*cdev = cdev_alloc();
	if (*cdev==NULL) {
		printk(KERN_ERR "cdev allocation failed...\n");
		return -1;
	}
	return 0;
}
/*
 * EXPORT_SYMBOL(cdriver_alloc);
 * 
 * This will export "cdriver_alloc" symbol into Module.symvers file.
 * If there is any undefined symbol warning during the module compilation
 * that means symbol is not exported into this file.
 * all symbols are available across multiple files and modules(dependency modules).
 */
int cdriver_open(struct inode *cinode, struct file *cfile) { 
	printk("open ...\n");
	return 0;
}

int cdriver_release(struct inode *cinode, struct file *cfile) {
	printk("release ...\n");
	return 0;
}

ssize_t cdriver_read(struct file *cfile, char __user *crbuf, size_t size, loff_t *croffset) {
	printk("read .... size :%d\n", size);
	return 0;
}

ssize_t cdriver_write(struct file *cfile, const char __user *crbuf, size_t size, loff_t *croffset) {
	printk("write .... size :%d\n", size);
	return size;
}
