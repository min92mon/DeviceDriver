obj-m	:= mmapcall_dev.o

KDIR  := /work/linux-3.0.22.fin
#KDIR	:= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
