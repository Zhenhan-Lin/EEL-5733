### EEL5733 assignment6

##### Compile & Test

+ To build the project, run `make` (alias of `make all`)
+ Run command below to test for each deadlock scenario

```terminal
sudo ./deadlock1
sudo ./deadlock2
sudo ./deadlock3
sudo ./deadlock4
```

+ To clean all the compile files, run `make clean`

##### DeadLock

1. When running in MODE 1, user cannot open the device more than one time due to line-50 in assignment6.c.

​	The output of deadlock1.c should be:

```stdout
Going to open device...
fd=3
Open succeeded!
Going to open device...
```

​	And stuck here.

2. When more than one threads running in MODE 2, all the threads that want to switch MODE 2 -> MODE 1 at the same time would be locked in a deadlock due to line-155. 

​	The output of deadlock2.c should be:

```stdout
Going to open device...
Open succeeded!
Going to MODE 2...
Going to create two threads in MODE 2..
Thread1: Going to open device...
Thread1: Open succedded!
Thread1: Going to set MODE 1...
Thread2: Going to open device...
Thread2: Open succedded!
Thread2: Going to set MODE 1...
```

​	And stuck here.

3. When more than 1 threads try to open device in MODE 1 at the same time, only one succeed and others stuck in trying to get sem2 due to line-50. At this point, if the succeeded thread wants to switch to MODE 2, it would be locked in a deadlock due to line-155 since other threads is waiting for sem2.

​	The output of deadlock3.c should be:

```
Creating two threads...
Thread1: Going to open device...
Thread1: fd=3
Thread1: Open succedded!
Thread2: Going to open device...
Thread1: Going to set MODE 2...
```

​	And stuck here.

4、When one thread is work in MODE 2 and is going to switch to MODE 1, another device opens and is also switch to MODE 1, the program stuck.

The output of deadlock3.c should be:

```
Creating two threads...
Thread2: Going to open device...
Thread2: Open succedded!
Thread2: Going to set MODE 2...
Thread2: Thread2 in MODE 2!
Thread1: Going to open device...
Thread1: Open succedded!
Thread1: Going to set MODE 1...
Thread2: Going to set MODE 1...
```



##### Data Race

1. In the case of MODE 1, when main process opens the device and creates more than one threads(child processes) which share file descriptor, the threads(child processes) could cause data race if more than one of them read or write at the same time. 

   *Critical Region:* 

   ```C
   # static ssize_t e2_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
   	down_interruptible(&devc->sem1);
   	if (devc->mode == MODE1) {
   	   up(&devc->sem1);
              if (*f_pos + count > ramdisk_size) {
                 printk("Trying to read past end of buffer!\n");
                 return ret;
              }
   	   ret = count - copy_to_user(buf, devc->ramdisk, count);
   	}
   	...
     return ret;
   }
   
   # static ssize_t e2_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
   	down_interruptible(&devc->sem1);
   	if (devc->mode == MODE1) {
   		up(&devc->sem1);
           if (*f_pos + count > ramdisk_size) {
               printk("Trying to read past end of buffer!\n");
               return ret;
           }
           ret = count - copy_from_user(devc->ramdisk, buf, count);
   	}
   	...
   	return ret;
   }
   ```

   *Accessed Data:*  **devc->ramdisk** -> out of lock's protection

   *Lock:* **device->sem1**

   *Reason:* Since lock 'sem1' does not protect the **devc->ramdisk** memory region from reading and writing operation, the operations among different threads can cause a data race.

2. When running MODE 2, the action from different users that open or close the device at the same time would be counting correctly through device->count2.

   *Critical Region:* 

   ```C
   # int e2_open(struct inode *inode, struct file *filp)
   down_interruptible(&devc->sem1);
   if (devc->mode == MODE1) {
       devc->count1++;
       up(&devc->sem1);
       down_interruptible(&devc->sem2);
       return 0;
   }
   else if (devc->mode == MODE2) {
     	devc->count2++;
   }
   up(&devc->sem1);
   # int e2_release(struct inode *inode, struct file *filp)
   down_interruptible(&devc->sem1);
   if (devc->mode == MODE1) {
       devc->count1--;
       if (devc->count1 == 1)
         wake_up_interruptible(&(devc->queue1));
       up(&devc->sem2);
   }
   else if (devc->mode == MODE2) {
       devc->count2--;
       if (devc->count2 == 1)
         wake_up_interruptible(&(devc->queue2));
   }
   up(&devc->sem1);
   ```

   *Accessed Data:*  **devc->count1**, **devc->count2**

   *Lock:* **device->sem1**

   *Reason:* Since lock 'sem1' protect the **devc->count**, which means it can only be accessed one by one, lock 'sem1' prevents the data race from occurring.

3. In the case of MODE 2, if multiple threads attempt to switch to mode 1, each operation would be protected by lock 'sem1', which prevents the data race from occurring.

​	*Critical Region:* 	

```c
# static long e2_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
case E2_IOCMODE1:
				down_interruptible(&devc->sem1);
				if (devc->mode == MODE1) {
				   up(&devc->sem1);
				   break;
				}
				if (devc->count2 > 1) {
				   while (devc->count2 > 1) {
				       up(&devc->sem1);
				       wait_event_interruptible(devc->queue2, (devc->count2 == 1));
				       down_interruptible(&devc->sem1);
				   }
				}
				devc->mode = MODE1;
        devc->count2--;
        devc->count1++;
				down_interruptible(&devc->sem2);
				up(&devc->sem1);
				break;
				
				default :
					pr_info("Unrecognized ioctl command\n");
					return -1;
					break;	
```

*Accessed Data:*  **devc->count1**, **devc->count2**

*Lock:* **device->sem1**

*Reason:* Since lock 'sem1' protect the **devc->count2/count1**, which means it can only be accessed one by one, lock 'sem1' prevents the data race from occurring.

4、When running MODE 2, the concurrent read or write operation would be prodected by lock 'sem1', which prevents the data race from occurring.

*Critical Region:* 

```c
# static ssize_t e2_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
	down_interruptible(&devc->sem1);
	if (devc->mode == MODE1) {
		...
	}
	else {
          if (*f_pos + count > ramdisk_size) {
             printk("Trying to read past end of buffer!\n");
             up(&devc->sem1);
             return ret;
          }
          ret = count - copy_to_user(buf, devc->ramdisk, count);
	  up(&devc->sem1);
	}
	return ret;
	
# static ssize_t e2_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
	down_interruptible(&devc->sem1);
	if (devc->mode == MODE1) {
		...
	}
	else {
        if (*f_pos + count > ramdisk_size) {
            printk("Trying to read past end of buffer!\n");
            up(&devc->sem1);
            return ret;
        }
        ret = count - copy_from_user(devc->ramdisk, buf, count);
		up(&devc->sem1);
	}
	return ret;
```

*Accessed Data:*  **devc->ramdisk**

*Lock:* **device->sem1**

*Reason:* Since lock 'sem1' protect the **devc->ramdisk**, which means that read or write operation can only accesse the shared memory region one by one, lock 'sem1' prevents the data race from occurring.