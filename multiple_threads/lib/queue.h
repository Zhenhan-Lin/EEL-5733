//
// Created by 林真含 on 2023/2/9.
//

#ifndef EEL5733_QUEUE_H
#define EEL5733_QUEUE_H

#include <stdlib.h>

typedef const char * ElementType;

struct Queue{
    ElementType *data;
    int front;
    int rear;
    int max_size;
    int size;
};

extern struct Queue queue;

void InitQueue(size_t);
void EnQueue(ElementType);
ElementType DeQueue();
int QIsFull();
int QIsEmpty();

#endif //EEL5733_QUEUE_H
