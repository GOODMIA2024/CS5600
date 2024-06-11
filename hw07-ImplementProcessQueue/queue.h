#include <stdio.h>
#include <stdlib.h>

/* Type Definition */
/*  Define struct process */ 
typedef struct process_t {
    int identifier;
    char *name;
    long runtime;
    int priority;
} process_t;


/* Define struct node */
typedef struct node_t {
    void *data;         // Does this represent any type?
    struct node_t *prev;       // Why there represent a * here?, * position, Why need a forced type change here 
    struct node_t *next;       // Using Struct?
} node_t;


/* Define a doubly linked list queue */
typedef struct queue_t {
    struct node_t *front;       // same as above, error: must use 'struct' tag to refer to type 'node_t'
    struct node_t *rear;
} queue_t;

void add2q(queue_t *queue, void *element);            //  Adds one instance of an `element` to the end of the passed `queue`. Implement the queue as a doubly-linked list, _i.e._, add forward and backward pointers to the `queue_t` structure defined above.
void* popQ(queue_t *queue);                           //  Removes and returns the data within the element at the front of the `queue`
process_t *rmProcess(queue_t *queue);                 // (* position 靠近函数名还是return值) Removes and returns the process with the highest priority in the `queue`. Note that we are presuming `process_t` types for this function but not the others.
int qsize(queue_t *queue);                            // Returns the number of elements in the queue; 0 if the queue is empty.