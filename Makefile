obj-m := cdriver.o
module-objs := cdriver_init.o cdriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

#obj-m ---> A make file symbol used by the kernel build system to determine which modules
#           should be build in current directory
