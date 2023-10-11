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
#define ramdisk_size (size_t) (16 * PAGE_SIZE)

#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IO(CDRV_IOC_MAGIC, 1)
#define E2_IOCMODE2 _IO(CDRV_IOC_MAGIC, 2)

static pthread_barrier_t barrier;

void *func1(){
    int fd;
    printf("Thread1: Going to open device...\n");
    fd = open(DEVICE_NAME, O_RDWR);
    if(fd == -1) {
        printf("File %s either does not exist or has been locked by another "
               "process\n", DEVICE_NAME);
        exit(-1);
    }else{
        printf("Thread1: Open succeeded!\n");
    }

    printf("Thread1: Going to set MODE 1...\n");
    pthread_barrier_wait(&barrier);
    ioctl(fd, E2_IOCMODE1, 0);
    close(fd);
    printf("Thread1: Thread1 in MODE 1!\n");
    pthread_exit(NULL);
}

void *func2(){
    int fd;

    printf("Thread2: Going to open device...\n");
    fd = open(DEVICE_NAME, O_RDWR);
    if(fd == -1) {
        printf("File %s either does not exist or has been locked by another "
               "process\n", DEVICE_NAME);
        exit(-1);
    }else{
        printf("Thread2: Open succeeded!\n");
    }

    printf("Thread2: Going to set MODE 1...\n");
    pthread_barrier_wait(&barrier);
    ioctl(fd, E2_IOCMODE1, 0);
    close(fd);
    printf("Thread2: Thread2 in MODE 1!\n");
    pthread_exit(NULL);
}

int main(){
    int fd;
    pthread_t tid[2];
    printf("Going to open device...\n");
    fd = open(DEVICE_NAME, O_RDWR);
    if(fd == -1) {
        printf("File %s either does not exist or has been locked by another "
               "process\n", DEVICE_NAME);
        exit(-1);
    }else{
        printf("Open succeeded!\n");
    }

    printf("Going to set MODE 2...\n");
    ioctl(fd, E2_IOCMODE2, 0);
    close(fd);

    pthread_barrier_init(&barrier, NULL, 2);
    printf("Going to create two threads in MODE 2...\n");
    pthread_create(&tid[0], NULL, &func1, NULL);
    pthread_create(&tid[1], NULL, &func2, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_exit(NULL);
}