obj-m = hellomisc.o

KDIR := /work/kernel-mds2450-3.0.22

all:
	make -C $(KDIR) M=$(PWD) modules
	$(CROSS_COMPILE)gcc misctest.c -o misctest
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -rf misctest
