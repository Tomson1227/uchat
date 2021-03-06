#include "queue.h"

// A utility function to create a new linked list node.
qnode_t *newNode(char *k) {
    qnode_t *temp = (qnode_t*)malloc(sizeof(qnode_t));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
queue_t *createQueue(void) {
    queue_t *q = (queue_t*)malloc(sizeof(queue_t));
    q->front = q->rear = NULL;
    return q;
}

// The function to add a key k to q
void enQueue(queue_t* q, char *k) {
        // Create a new LL node
    qnode_t *temp = newNode(k);
        // If queue is empty, then new node is front and rear both
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
        // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

// Function to remove a key from given queue q
char *deQueue(queue_t *q) {
        // If queue is empty, return NULL.
    if (q->front == NULL) {
        return NULL;
    }
        // Store previous front and move front one node ahead
    char *res = q->front->key;
    qnode_t* temp = q->front;
    q->front = q->front->next;
        // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return res;
}

bool QueueisEmpty(queue_t *q) {
    if (NULL == q->front || NULL == q->rear) {
        return true;
    } else {
        return false;
    }
}
