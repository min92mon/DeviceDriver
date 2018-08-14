obj-m := blocking_keypad.o

#KDIR	:= /lib/modules/$(shell uname -r)/build
#KDIR  := /work/linux/linux-2.6.21
#KDIR  := /work/linux-3.0.22.fin
#KDIR  := /work/kernel-mds2450-3.0.22
KDIR  := /work/kernel-mds2450-3.0.22

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules
	cp blocking_keypad.ko /nfs/rootfs/root
	
clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean

