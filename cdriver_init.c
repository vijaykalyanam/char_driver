#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>


MODULE_LICENSE("GPL"); 

int cdrvier_alloc(struct cdev **cdev) {

	*cdev = cdev_alloc();
	if (!(*cdev)) {
		printk(KERN_ERR "cdev allocation failed...\n");
		return -1;
	}
	printk(KERN_ALERT "function called\n");
	return 0;
}
