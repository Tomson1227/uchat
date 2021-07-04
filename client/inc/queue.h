#ifndef QUEUE_H
#define QUEUE_H

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Typedefs */
typedef struct QNode {
    char *key;
    struct QNode* next;
}          qnode_t;

typedef struct Queue {
    qnode_t *front;
    qnode_t *rear;
}           queue_t;

qnode_t *newNode(char *k);
queue_t *createQueue(void);
void enQueue(queue_t* q, char *k);
char *deQueue(queue_t *q);
bool QueueisEmpty(queue_t *q);

#endif /* QUEUE_H */
