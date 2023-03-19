//
// Created by 林真含 on 2023/2/9.
//
#include "queue.h"
#include "util.h"

struct Queue * queue;

void InitQueue(size_t max_size){
    // mmap-ed memory region
    // +-------+--------------------+
    // | queue | n * 128            |
    // +-------+--------------------+
    // ^ p     ^ p + sizeof(*queue)
    size_t s = sizeof(*queue) + max_size * sizeof(ElementType) /* max_size * 128 */;
    void *p = mmap(NULL, s, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    queue = p;
    queue->data = p + sizeof(*queue);   // Put data after all the pointers
    if(!queue->data) errExit("No Space for Buffer");

    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->max_size = max_size;
}

void EnQueue(ElementType content){
    queue->data[queue->rear] = content;
    queue->rear = (queue->rear + 1) % queue->max_size;
    queue->size++;
}

ElementType DeQueue(){
    ElementType e = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->max_size;
    queue->size--;
    return e;
}

int QIsFull(){
    return queue->size == queue->max_size;
}

int QIsEmpty(){
    return queue->size == 0;
}