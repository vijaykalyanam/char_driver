/*
 * 
 * 
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include "headers.h"

struct mydriver drv;
struct task_struct *task;

/* 
 * Advantages of using ENUM over #DEFINE 
 * -> Easy Maintenance
 * -> In case of Removing or changing value, less error prone.
 *
 */

enum state {
	KTHREAD_STOP,
	KTHREAD_CREATE,
	KTHREAD_START,
	KTHREAD_DELAY,
	KTHREAD_UNKNOWN = 0xFF,
};

static ssize_t task6_read_id(struct file *file, char __user *buf,
		size_t count, loff_t *ppos)
{
	printk("Working on Read Operation\n");
	return 0;
	//return simple_read_from_buffer(buf, count, ppos, id, sizeof(id));
}

static ssize_t task6_write_id(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos)
{
#if 0
	ssize_t num_bytes;
	char kbuf[ID_SIZE];

	num_bytes = simple_write_to_buffer(kbuf, ID_SIZE, ppos, buf, count);

	if (num_bytes != ID_SIZE ||
			memcmp(id, kbuf, ID_LEN))
		return -EINVAL;
	return ID_SIZE;
#else
	printk("Working on Write operation\n");
#endif
	return 1;
}

static long task6_ioctl(struct file *file, unsigned int cmd, unsigned long data)
{

	switch(cmd) {
		case KTHREAD_CREATE:
			printk("[%s] KTHREAD_CREATE\n", __func__);
			if (drv.thread_started) {
				printk("Thread already running ...\n");
			} else {
				task = create_thread(&drv);
				if (task)
					drv.thread_started = 1;
			}
			break;
		case KTHREAD_START: 
			printk("[%s] KTHREAD_START\n", __func__);
			if (drv.thread_created && !drv.thread_started) {
				printk("Starting thread :%d\n",start_mythread(task));
			} else {
				printk("Thread not yet created\n");
			}				
			break;
		case KTHREAD_STOP: drv.stop_thread = 1;
				   printk("KTHREAD Action :%u\n", cmd);
				   break;
		default : printk("Unknown IOCTL Command %u\n", cmd);
	}

	return 0;
}

const struct file_operations task6_fops = {
	.owner	 = THIS_MODULE,
	.read    = task6_read_id,
	.write   = task6_write_id,
	.unlocked_ioctl	 = task6_ioctl,
};

static struct miscdevice task6_dev = {
	.minor  = MISC_DYNAMIC_MINOR,
	.name   = "saanvika",
	.fops   = &task6_fops,
};

static void __exit task1_exit(void)
{
	pr_info("Misc Char driver with minor number [%d] unloaded\n",
			task6_dev.minor);
	misc_deregister(&task6_dev);
}

static int __init task1_init(void)
{
	int ret = -1;

	ret = misc_register(&task6_dev);

	if (ret == 0) {
		pr_info("Misc Char driver with minor number [%d] registered\n",
				task6_dev.minor);
		memset(&drv, 0, sizeof(drv));
		memcpy(drv.name, "mythread", sizeof("mythread"));
	} else {
		pr_info("Misc Char driver registerion failed with error [%d]",
				ret);
	}

	return ret;
}

module_init(task1_init);
module_exit(task1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SAANVIKA");
MODULE_DESCRIPTION("Task6: Miscellaneous char device driver");
