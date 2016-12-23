#include <linux/cdev.h>
#include <linux/kernel.h>

int cdrvier_init(struct cdev **cdev) {

	*cdev = cdev_alloc();
	if (!(*cdev)) {
		printk(KERN_ERR "cdev allocation failed...\n");
		return -1;
	}

	return 0;
}
