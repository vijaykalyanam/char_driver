/*
 *
 * Kernel THREADS are traked using task_struct data structure
 * Each thread has one name associated with it.
 * Once Thread creation is success, then next we need to wake it up / tell kernel to start running thread.

 * can’t access the user address space (even with copy_from_user, copy_to_user)
 * because a thread kernel does not have a user address space
 *
 * can’t implement busy wait code that runs for a long time;
 *  if the kernel is compiled without the preemptive option,
 *  that code will run without being preempted by other kernel threads or user processes thus hogging the system
    can call blocking operations
 *
 * can use spinlocks, but if the hold time of the lock is significant, it is recommended to use mutexes
 *
 */
 
#include <linux/kthread.h>
#include "headers.h"

extern struct mydriver drv;

int mythread(void *data)
{
	printk("MYTHREAD STARTED\n");
	struct mydriver *d;

	d = (struct mydriver *)data;

	if (data == NULL)
		return -1;

	printk("mythread started ......"
			"First Open Call And it stays until user kills\n");
	while(1) {
		/* TODO: Use Atomic Operations/Atomic Bits */
		if (d->stop_thread) {
			printk("Stopping Thread\n");
			do_exit(0);
		}
		/* Cannot Use mdelay/udelay/ndelay with larger values */
		/* They will can NMI CPU hook up */ 
		/* Delay functions pauses the execution */ 
		//mdelay(5000);

		mdelay(1000);
		yield();
		printk(" Thread state :%d\n", (d->thread_stopped) ? 0 : 1);
	}

	return 0;
}

struct task_struct *create_thread(struct mydriver *drv)
{
	struct task_struct *t;

	if (drv == NULL)
		return NULL; 

	t = kthread_create(mythread, drv, drv->name);

	if (t)
		return t; 

	return NULL; 
}

int start_mythread(struct task_struct *t)
{

	if (t == NULL)
		return -1;
	return wake_up_process(t);
}
