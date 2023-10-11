obj-m += assignment6.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o deadlock1 deadlock1.c -lpthread
	gcc -o deadlock2 deadlock2.c -lpthread
	gcc -o deadlock3 deadlock3.c -lpthread
	gcc -o deadlock4 deadlock4.c -lpthread

deadlock1: clean3
	gcc -o deadlock1 deadlock1.c -lpthread

deadlock2:
	gcc -o deadlock2 deadlock2.c -lpthread

deadlock3:
	gcc -o deadlock3 deadlock3.c -lpthread

deadlock4: clean2
	gcc -o deadlock4 deadlock4.c -lpthread

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf deadlock1 deadlock2 deadlock3 deadlock4

clean2:
	rm -rf deadlock4

clean3:
	rm -rf deadlock1
