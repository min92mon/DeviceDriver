obj-m := keysample.o
keysample-objs	:= keyint_bottom_dd.o
#keysample-objs	:= keyint_bottom.o

EXTRA_CFLAGS += -I./include

#KDIR 	:= /work/kernel-mds2450-3.0.22
KDIR 	:= /work/linux-3.0.22.fin


default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

