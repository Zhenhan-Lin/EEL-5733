//
// Created by 林真含 on 2023/2/10.
//

#include "queue.h"
#include "util.h"
#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t producer = PTHREAD_COND_INITIALIZER;
static pthread_cond_t consumer = PTHREAD_COND_INITIALIZER;


// The function thread_puts() writes the string s, and a terminating newline character, to the stream stdout.
void thread_puts(const char *str){
    // producer
//    int s = pthread_mutex_lock(&model.lock);
    int s = pthread_mutex_lock(&lock);
    if(s!=0)
        errExit("producer pthread mutex lock error");
    // check if queue is full
    while(QIsFull())
        pthread_cond_wait(&producer, &lock);
    EnQueue(str);
    // Release the lock and send signal to consumer
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&consumer);
}

// Get the string from thread_puts
const char * thread_getline(){
    // consumer
    int s = pthread_mutex_lock(&lock);
    if(s!=0)
        errExit("consumer pthread mutex lock error");
    // check if queue is empty
    while(QIsEmpty())
        pthread_cond_wait(&consumer, &lock);
    const char * str = DeQueue();
    // Release the lock and send signal to consumer
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&producer);
    return str;
}