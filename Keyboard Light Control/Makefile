obj-m += usbkbd.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod usbkbd.ko

config:
	sudo bash config.sh

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	sudo rmmod usbkbd
