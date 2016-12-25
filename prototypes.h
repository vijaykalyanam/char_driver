int cdriver_alloc(struct cdev **);
ssize_t cdriver_read(struct file *, char __user *, size_t, loff_t *);
ssize_t cdriver_write(struct file *, const char __user *, size_t, loff_t *);
int cdriver_open (struct inode *, struct file *);
int cdriver_release (struct inode *, struct file *);
