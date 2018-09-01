/*
 *
 * Kernel THREADS are traked using task_struct data structure
 * Each thread has one name associated with it.
 * Once Thread creation is success, then next we need to wake it up / tell kernel to start running thread.
 */
 
#include <linux/kthread.h>
#include "headers.h"

extern struct mydriver drv;

int mythread(void *data)
{
	struct mydriver *d;

	d = (struct mydriver *)data;

	if (data == NULL)
		return -1;

	printk("mythread started ......"
			"First Open Call And it stays until user kills\n");
	while(1) {
		/* TODO: Use Atomic Operations/Atomic Bits */
		if (d->stop_thread)
			do_exit(0);
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
