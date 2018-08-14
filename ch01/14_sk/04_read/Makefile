obj-m   := minmon.o

KDIR    := /home/parallels/Work/mango24r2/kernel/kernel 

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
	cp minmon.ko ~/Work/rootfs/root
clean:	
	make -C $(KDIR) SUBDIRS=$(PWD) clean
	rm ~/Work/rootfs/root/minmon.ko
