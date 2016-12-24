#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/module.h>


MODULE_LICENSE("GPL"); 

int cdriver_alloc(struct cdev **);
int cdriver_alloc(struct cdev **cdev) {

	*cdev = cdev_alloc();
	if (!(*cdev)) {
		printk(KERN_ERR "cdev allocation failed...\n");
		return -1;
	}
	printk(KERN_ALERT "function called\n");
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
