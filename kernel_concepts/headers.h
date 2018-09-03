#include <linux/completion.h>
#include <linux/semaphore.h>
#include <linux/delay.h>

struct mydriver {
	char name[32];	
	struct semaphore slock; 
	struct completion cvar;
	struct task_struct ts;
	bool thread_created;
	bool thread_started; 
	bool stop_thread;
	bool thread_stopped;
};

int mythread(void *data);
struct task_struct *create_thread(struct mydriver *drv);
int start_mythread(struct task_struct *t);
