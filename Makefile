TARGET = chardriver

obj-m := $(TARGET).o
$(TARGET)-objs := cdriver_init.o cdriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
