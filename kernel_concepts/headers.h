#include <linux/completion.h>
#include <linux/semaphore.h>
#include <linux/delay.h>
#include <linux/wait.h>

struct mydriver {
	char name[32];	
	struct semaphore slock; 
	bool use_completion;
	bool lock_completion;
	struct completion cvar;
	bool use_wait_queue;
	bool addressed_event;
	wait_queue_head_t waitq1; /* For Kernel Module */
	bool posted_event;
	wait_queue_head_t waitq2; /* For Kernel Thread */
	struct task_struct ts;
	bool thread_created;
	bool thread_started; 
	bool stop_thread;
	bool thread_stopped;
	bool lock_semaphore;
};

int mythread(void *data);
struct task_struct *create_thread(struct mydriver *drv);
int start_mythread(struct task_struct *t);
