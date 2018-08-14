obj-m += kerneltimer.o

#KDIR  := /work/kernel-mds2450-3.0.22
KDIR  := /work/linux-3.0.22.fin

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
