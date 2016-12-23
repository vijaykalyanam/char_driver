#include <linux/init.h>           // INIT MACROs
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/kernel.h>         // Contains types, macros, functions(printk) for the kernel
#include <linux/moduleparam.h>    // Kernel module param


#include <linux/types.h>
#include <linux/kdev_t.h>         // For MKDEV(int major, int minor)
#include <linux/cdev.h>

#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function
#include <linux/device.h>         // Header to support the kernel Driver Model


static char *name = "cvijay";
module_param(name, charp, S_IRUSR);

struct file_operations {
	.owner = THIS_MODULE,
}cdriver_fops;

static int __init cdriver_init(void) {

struct cdev *cdev = alloc_cdev();

	return 0;
}

static void __exit cdriver_exit(void) {

	printk(KERN_ALERT "cdriver exit\n");
	return;
}

// __init and __exit macro tells the compiler those are load, unload functions. So that compiler place them in separate area in ELF format.
module_init(cdriver_init);
module_exit(cdriver_exit);

/*
 * We need to include our licence in the module during compilation.
 * Otherwise module will fail with error saying that kernel taint because of untrusted license.
 */
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Vijay Kalyanam");
MODULE_VERSION("23.12.2016");
MODULE_DESCRIPTION("This is just a Char driver");
