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
	KTHREAD_STOP = 10,
	KTHREAD_CREATE,
	KTHREAD_START,
	KTHREAD_EVENT,
	KTHREAD_EVENT_SEMAPHORE,
	KTHREAD_EVENT_COMPLETION,
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
	printk("task6_ioctl : cmd :%d\n", cmd);
	switch(cmd) {
		case KTHREAD_CREATE:
			printk("KTHREAD_CREATE\n");
			if (drv.thread_started) {
				printk("Thread already running ...\n");
			} else {
				task = create_thread(&drv);
				if (task) {
					drv.thread_created = 1;
					drv.thread_stopped = 1;
				}
			}
			break;
		case KTHREAD_START: 
			printk("KTHREAD_START\n");
			printk("[%s] KTHREAD_START\n", __func__);
			if (drv.thread_created && !drv.thread_started) {
				drv.thread_stopped = false;
				printk("Starting thread :%d\n",start_mythread(task));
			} else {
				printk("Thread not yet created\n");
			}				
			break;

		case KTHREAD_EVENT_COMPLETION:
			if (!drv.thread_started || drv.thread_stopped) {
				printk("Thread stopped/not created\n");
				break;
			}
			if (!drv.use_completion) {
				printk("Completion variable is not initialized\n");
				break;
			}
			printk("KTHREAD_RAISE event at %ld\n", jiffies);
			up(&drv.slock);
			/* uninterruptible wait */
			wait_for_completion(&drv.cvar);
			printk("KTHREAD Task Completed %ld\n", jiffies);
		case KTHREAD_EVENT:
		case KTHREAD_EVENT_SEMAPHORE:
			if (!drv.thread_started || drv.thread_stopped) {
				printk("Thread stopped/not created\n");
				break;
			}
			printk("KTHREAD_RAISE event\n");
			up(&drv.slock);
			break;

		case KTHREAD_STOP:
			if (!drv.thread_stopped) {
				printk("KTHREAD_STOP\n");
				drv.stop_thread = 1;
				up(&drv.slock);
			} else {
				printk("KTHREAD_STOP -> Thread alread stopped\n");
			}
			break;
		default : printk("Unknown IOCTL Command %u\n", cmd);
	}

	return 0;
}

/* lock is true, calling thread waits for event result 
 * from the called thread */

unsigned int lock = 1;
//module_param(lock, unsigned int, 1);
 
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
		memcpy(drv.name, "mythread1", sizeof("mythread1"));

		if (lock) {
			init_completion(&drv.cvar);
			drv.use_completion = true;
		} else {
			sema_init(&drv.slock, 1);
			down(&drv.slock);
		}
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
