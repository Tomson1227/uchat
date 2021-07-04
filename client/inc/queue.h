#ifndef QUEUE_H
#define QUEUE_H

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>

/* Typedefs */
typedef struct QNode {
    int key;
    struct QNode* next;
}          qnode_t;

typedef struct Queue {
    qnode_t *front;
    qnode_t *rear;
}           queue_t;

qnode_t *newNode(int k);
queue_t *createQueue(void);
void enQueue(queue_t* q, int k);
int deQueue(queue_t *q);
int QueueisEmpty(queue_t *q);

#endif /* QUEUE_H */
