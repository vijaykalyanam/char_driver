#include <linux/init.h>           // INIT MACROs
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/kernel.h>         // Contains types, macros, functions(printk) for the kernel
#include <linux/moduleparam.h>    // Kernel module param

#include <linux/cdev.h>
#include <linux/fs.h>

#include "prototypes.h"
/*
 * We need to include our licence in the module during compilation.
 * Otherwise module will fail with error saying that kernel taint because of untrusted license.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vijay Kalyanam");
MODULE_VERSION("23.12.2016");
MODULE_DESCRIPTION("This is just a Char driver");


static char *name = "cvijay";
module_param(name, charp, S_IRUSR);
unsigned int major=117, minor=1;

struct cdev *cdev = NULL;
static const struct file_operations cdriver_fops = {
	.owner = THIS_MODULE,
	.open  = cdriver_open,
	.release = cdriver_release,
	.read = cdriver_read,
	.write = cdriver_write,
};

static int __init cdriver_init(void) {

	int rc;
	dev_t dev = 0;
	rc = cdriver_alloc(&cdev);
	if (!rc) {
		dev = MKDEV(major, minor);
		printk(KERN_INFO "chardriver :dev id :%d\n", dev);

		cdev->owner = THIS_MODULE;
		cdev->dev = dev;
		cdev_init(cdev, &cdriver_fops);
		printk(KERN_INFO "CDEV Initialized\n");

		rc = cdev_add(cdev, dev, minor);
		if (rc == 0) { 
			printk(KERN_INFO "char device regiestered with kernel :%d\n", dev);
			return 0;
		}
		printk(KERN_ERR "cdev registration with kernel failed rc :0x%x\n",rc);
		cdev_del(cdev);
	}
	return -1;
}

static void __exit cdriver_exit(void) {

	if (cdev) cdev_del(cdev);
	printk(KERN_INFO "cdev with dev id :%d unloaded\n", cdev->dev);
	return;
}

/*
 * __init and __exit macro tells the compiler those are load, unload functions.
 * So that compiler place them in separate area in ELF format.
 */
module_init(cdriver_init);
module_exit(cdriver_exit);
