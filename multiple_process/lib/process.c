//
// Created by 林真含 on 2023/2/10.
//

#include "queue.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

static pthread_mutex_t * lock;
static pthread_cond_t * producer;
static pthread_cond_t * consumer;

pthread_mutexattr_t attr_lock;
pthread_condattr_t attr;


void process_init(){
    size_t s = sizeof(pthread_mutex_t) + 2 * sizeof(pthread_cond_t);
    void *p = mmap(NULL, s, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    lock = p;
    producer = p + 1;
    consumer = producer + 1;
    pthread_mutexattr_init(&attr_lock);
    pthread_mutexattr_setpshared(&attr_lock, PTHREAD_PROCESS_SHARED);
    pthread_condattr_init(&attr);
    pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(lock, &attr_lock);
    pthread_cond_init(producer, &attr);
    pthread_cond_init(consumer, &attr);
    printf("%p %p %p", lock, producer, consumer);
}

// The function thread_puts() writes the string s, and a terminating newline character, to the stream stdout.
void process_puts(const char *str){
    // producer
//    int s = pthread_mutex_lock(&model.lock);
    static ElementType _str;
    strcpy(_str.str, str);
    int s = pthread_mutex_lock(lock);
    if(s!=0)
        errExit("producer pthread mutex lock error\n");
    // check if queue is full
    while(QIsFull())
        pthread_cond_wait(producer, lock);
    EnQueue(_str);
    // Release the lock and send signal to consumer
    pthread_mutex_unlock(lock);
    pthread_cond_signal(consumer);
}

// Get the string from thread_puts
const char * process_getline(){
    // consumer
    int s = pthread_mutex_lock(lock);
    if(s!=0)
        errExit("consumer pthread mutex lock error\n");
    // check if queue is empty
    while(QIsEmpty())
        pthread_cond_wait(consumer, lock);
    ElementType * str = malloc(sizeof(ElementType));
    * str = DeQueue();
    // Release the lock and send signal to consumer
    pthread_mutex_unlock(lock);
    pthread_cond_signal(producer);
    return str->str;
}