//
// Created by 林真含 on 2023/2/10.
//

#include "queue.h"
#include "util.h"
#include <stdio.h>
#include <pthread.h>

struct Model {
    pthread_mutex_t lock;
    pthread_cond_t consumer, producer;
} model = {
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_COND_INITIALIZER,
        PTHREAD_COND_INITIALIZER
};

// The function thread_puts() writes the string s, and a terminating newline character, to the stream stdout.
void thread_puts(const char *str){
    // producer
    int s = pthread_mutex_lock(&model.lock);
    if(s!=0)
        errExit("producer pthread mutex lock error");
    // check if queue is full
    while(QIsFull())
        pthread_cond_wait(&model.producer, &model.lock);
    EnQueue(str);
    // Release the lock and send signal to consumer
    pthread_mutex_unlock(&model.lock);
    pthread_cond_signal(&model.consumer);
}
// Get the string from thread_puts
const char * thread_getline(){
    // consumer
    int s = pthread_mutex_lock(&model.lock);
    if(s!=0)
        errExit("consumer pthread mutex lock error");
    // check if queue is empty
    while(QIsEmpty())
        pthread_cond_wait(&model.consumer, &model.lock);
    const char * str = DeQueue();
    // Release the lock and send signal to consumer
    pthread_mutex_unlock(&model.lock);
    pthread_cond_signal(&model.producer);
    return str;
}