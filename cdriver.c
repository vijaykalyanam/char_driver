#include <linux/init.h>           // INIT MACROs
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/kernel.h>         // Contains types, macros, functions(printk) for the kernel
#include <linux/moduleparam.h>    // Kernel module param

#include <linux/cdev.h>

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

int cdriver_alloc(struct cdev **);

static int __init cdriver_init(void) {

	struct cdev *cdev = NULL;

	printk(KERN_ERR "cdriver init\n");
	cdriver_alloc(&cdev);

	if (cdev) cdev_del(cdev);

	return 0;
}

static void __exit cdriver_exit(void) {

	printk(KERN_ALERT "cdriver exit\n");
	return;
}

/*
 * __init and __exit macro tells the compiler those are load, unload functions.
 * So that compiler place them in separate area in ELF format.
 */
module_init(cdriver_init);
module_exit(cdriver_exit);
