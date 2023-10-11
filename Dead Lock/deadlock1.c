#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/wait.h>

#define DEVICE_NAME "/dev/a5"

#define CDRV_IOC_MAGIC 'Z'


int main(){
    int fd1, fd2;

    printf("Going to open device...\n");
    fd1 = open(DEVICE_NAME, O_RDWR);
    printf("fd=%d\n", fd1);
    if(fd1 == -1) {
        printf("File %s either does not exist or has been locked by another "
               "process\n", DEVICE_NAME);
        exit(-1);
    }else{
        printf("Open succeeded!\n");
    }

    printf("Going to open device...\n");
    fd2 = open(DEVICE_NAME, O_RDWR);
    printf("fd=%d\n", fd2);
    if (fd2 == -1){
        printf("File %s either does not exist or has been locked by another "
               "process\n", DEVICE_NAME);
        exit(-1);
    }else{
        printf("Open succeeded!\n");
    }

    close(fd1);
    close(fd2);
    return 0;
}