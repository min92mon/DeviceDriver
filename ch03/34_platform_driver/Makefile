obj-m := keysample.o
keysample-objs	:= keyint_bottom_dd_platform.o

EXTRA_CFLAGS += -I./include

#KDIR	:= /work/M2_BSP/linux-2.6.21-mep2440
KDIR  := /work/linux-3.0.22.fin


default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
#	cp -ra keysample.ko /opt/4.2.4-arm/arm-generic-linux-gnu/sys-root/root/
	cp -ra keysample.ko /nfs/rootfs/root
	
clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
	rm -rf *.bak
	


