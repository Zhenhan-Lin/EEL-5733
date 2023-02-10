//
// Created by 林真含 on 2023/2/9.
//
#include "queue.h"
#include "util.h"

struct Queue queue;

void InitQueue(size_t max_size){
    queue.data = malloc(max_size * sizeof(ElementType));
    if(!queue.data) errExit("No Space for Buffer");

    queue.front = 0;
    queue.rear = 0;
    queue.size = 0;
    queue.max_size = max_size;
}

void EnQueue(ElementType content){
    queue.data[queue.rear] = content;
    queue.rear = (queue.rear + 1) % queue.max_size;
    queue.size++;
}

ElementType DeQueue(){
    ElementType e = queue.data[queue.front];
    queue.front = (queue.front + 1) % queue.max_size;
    queue.size--;
    return e;
}

int QIsFull(){
    return queue.size == queue.max_size;
}

int QIsEmpty(){
    return queue.size == 0;
}