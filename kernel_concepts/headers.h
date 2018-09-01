
struct mydriver {
	char name[32];	
	bool thread_created;
	bool thread_started; 
	bool stop_thread;
};

int mythread(void *data);
struct task_struct *create_thread(struct mydriver *drv);
int start_mythread(struct task_struct *t);
