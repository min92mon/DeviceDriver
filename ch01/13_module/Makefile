obj-m += hello_param.o

KDIR	:= /work/linux-3.0.22.fin

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean

