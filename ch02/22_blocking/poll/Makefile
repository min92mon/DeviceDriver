obj-m := sk.o

KDIR    := /work/kernel-mds2450-3.0.22

all:
		make -C $(KDIR) SUBDIRS=$(PWD) modules
		cp sk.ko /nfs/rootfs/root

clean:
		make -C $(KDIR) SUBDIRS=$(PWD) clean
