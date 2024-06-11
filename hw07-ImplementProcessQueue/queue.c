#include <stdio.h>
#include "queue.h"

void add2q(queue_t *queue, void *element) {
    // 1. create a new node to hold the element passed in
    // node is a pointer pointing to the mempry block which holds a node_t struct
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->data = element;  // -> 代表什么？
    new_node->prev = NULL;
    new_node->next = NULL;

    // 2. push the new node into queue
        // if q is empty, make front and rear both pointing to this new node
    if (queue->front == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        // if q is not empty, append this new node to the rear of the current q
        queue->rear->next = new_node;                   // point q rear to new node
        new_node->prev = queue->rear;                   // point new node prev to q rear
        queue->rear = new_node;                         // update the queue rear pointer
    }
}

void* popQ(queue_t *queue) {                            // 不太理解这个函数的作用
    if (queue->front == NULL) {
        fprintf(stderr, "Cannot pop element from queue when queue is empty.\n");
        return NULL;
    } else {
        void *target = queue->front->data;
        node_t *temp = queue->front;
        queue->front = queue->front->next;              // Move the pointer
        if (queue->front == NULL) {
            queue->rear = NULL;
        }
        free(temp);
        return target;
    }
}

process_t *rmProcess(queue_t *queue) {
    // If queue is empty, return null
    if (queue->front == NULL) {
        fprintf(stderr, "Cannot remove process for a NULL queue.\n");
        return NULL;
    }

    // If queue is not empty
    // traverse the queue to find the process of highest priority
    node_t *current = queue->front;
    node_t *high_priority = queue->front;
    while (current != NULL) {
        process_t *curPtr = (struct process_t*)current->data;                     // Convert void* pointer to another struct in C
        process_t *high_priorityPtr = (struct process_t*)high_priority->data;
        if (curPtr->priority > high_priorityPtr->priority) {                         // Using . to access the variable inside a struct
            high_priority = current;
        }
        current = current->next;
    }

    // Update the previous node
    // if hi_pri is the first node in the queue, update the queue front as its current front next
    if (high_priority == queue->front) {
        queue->front = queue->front->next;
    } else {
    // if hi_pri is not the first node in the queue
        high_priority->prev->next = high_priority->next;
    }
    
    // Update the next node
    if (high_priority == queue->rear) {
        queue->rear = queue->rear->prev;
    } else {
        high_priority->next->prev = high_priority->prev;
    }

    // Cast void pinter to a pointer of type 'struct process_t'
    process_t *process = (struct process_t*)high_priority->data;
    
    // Free the memory of removed node
    free(high_priority);
    return process;
}

int qsize(queue_t *queue) {
    // Check if queue is NULL
    if (queue->front == NULL) {
        fprintf(stderr, "Cannot count the size of NULL queue.\n");
        return -1;
    }
    // traverse through the queue (doubly-linked list) to count the number of nodes
    int count = 0;                      // Need to be assigned some value, otherwise very small number
    node_t *current = queue->front;
    while (current != NULL ){
        count++;
        current = current->next;
    }
    return count;
}


