obj-m   := minmon.o

KDIR    := ~/Work/mango24r2/kernel/kernel 

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
	$(CROSS_COMPILE)gcc -o minmon_app minmon_app.c
	cp minmon.ko ~/Work/rootfs/root
	cp minmon_app ~/Work/rootfs/root

clean:	
	make -C $(KDIR) SUBDIRS=$(PWD) clean
	rm ~/Work/rootfs/root/minmon.ko
	rm -rf minmon_app
	rm -rf ~/Work/rootfs/root/minmon_app
