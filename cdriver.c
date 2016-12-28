#include <linux/init.h>           // INIT MACROs
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/kernel.h>         // Contains types, macros, functions(printk) for the kernel
#include <linux/moduleparam.h>    // Kernel module param
#include <linux/version.h>

#include <linux/device.h>
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


static char *name = "char_device", *class_name = "char_class";
module_param(name, charp, S_IRUSR);

static struct class *cclass;
static struct device *cdevice;

dev_t dev;
unsigned int major, minor=1, count=1;

struct cdev *cdev;
static const struct file_operations cdriver_fops = {
	.owner = THIS_MODULE,
	.open  = cdriver_open,
	.release = cdriver_release,
	.read = cdriver_read,
	.write = cdriver_write,
};

static int __init cdriver_init(void) {

	int rc=0;
	rc = cdriver_alloc(&cdev);
	if (!rc) {
		rc = alloc_chrdev_region(&dev, minor, count, name); 
		if (rc) {
			printk(KERN_ERR "%s char device allocation failed\n", name);
			goto del_cdev;
		}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,16)
		rc = register_chrdev_region(dev, count, name);
		if (rc) {
			printk(KERN_ERR "%s char device registeration failed\n", name);
			goto del_cdev;
		}
#else
/* This is a #define to keep the compiler from merging different
 * instances of the __key variable
 * #define class_create(owner, name)
 * ({
 *       static struct lock_class_key __key;
 *       __class_create(owner, name, &__key);
 * })
 * extern void class_destroy(struct class *cls);
 */

//new class will be created under /sys/class/
		cclass = class_create(THIS_MODULE, class_name);
		if (cclass==NULL) {
			printk(KERN_ALERT "Class creation failed\n");
			goto del_cdev;
		} else {
			printk (KERN_INFO "Class created\n");
		}
/*
 * struct device *device_create(struct class *cls, struct device *parent,
 *                            dev_t devt, void *drvdata,
 *                            const char *fmt, ...);
 *  extern void device_destroy(struct class *cls, dev_t devt);
 */

//New device (char device) will be created in /dev/
		cdevice = device_create(cclass, NULL, dev, NULL, name, NULL);
		if (cdevice) {
			printk(KERN_INFO "Device creation success for cvijay, major :%u minor :%u\n",
					MAJOR(dev), MINOR(dev));
		} else {
			printk(KERN_ALERT "Device creation failed\n");
			goto del_class;
		}
#endif
		cdev->owner = THIS_MODULE;
		cdev->dev = dev;
		cdev_init(cdev, &cdriver_fops);
		printk(KERN_INFO "CDEV Initialized\n");

		rc = cdev_add(cdev, dev, minor);
		if (rc == 0) {
			printk(KERN_INFO " %s char device is up. Major no :%u minor :%u\n",
					name, MAJOR(dev), MINOR(dev));
			return 0;
		}
		device_destroy(cclass, dev);
del_class:
		class_destroy(cclass);
del_cdev:
		printk(KERN_ERR "cdev registration with kernel failed rc :0x%x\n",rc);
		cdev_del(cdev);
	}
	return -1;
}

static void __exit cdriver_exit(void) {

	if(cclass)
		class_destroy(cclass);
	if(cdevice)
		device_destroy(cclass, dev);
		cdev_del(cdev);
	if (dev)
		unregister_chrdev_region(dev, count);

	return;
}

/*
 * __init and __exit macro tells the compiler those are load, unload functions.
 * So that compiler place them in separate area in ELF format.
 */
module_init(cdriver_init);
module_exit(cdriver_exit);
