/**
 * queue.c / Implement Process queue
 * Minjia Tao / CS5600 / Northeastern University
 * Spring 2024 / Feb 5, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(int argc, char *argv[]) {                  // main函数记住怎么写
    // 1. Create a new test queue
    queue_t testqueue;                               // queue_t *testqueue;
    testqueue.front = NULL;                         // acess the variable via ., which is pointer, if get data , then dereference.
    testqueue.rear = NULL;
    printf("Testqueue has been initialized.\n");

    // 2. Update the test queue
    process_t process1 = {1, "ProcessOne", 120000, 3};
    process_t process2 = {2, "ProcessTwo", 250000, 5};
    process_t process3 = {3, "ProcessThree", 75000, 2};
    process_t process4 = {4, "ProcessFour", 300000, 4};

    // Add processes to the test queue
    add2q(&testqueue, &process1);
    printf("%s has been added to the queue. Queue Size: %d\n", process1.name, qsize(&testqueue));

    add2q(&testqueue, &process2);
    printf("%s has been added to the queue. Queue Size: %d\n", process2.name, qsize(&testqueue));

    add2q(&testqueue, &process3);
    printf("%s has been added to the queue. Queue Size: %d\n", process3.name, qsize(&testqueue));

    add2q(&testqueue, &process4); // & 和*有什么不同？
    printf("%s has been added to the queue. Queue Size: %d\n", process4.name, qsize(&testqueue));


    // 3. Print out the contents of the queue
    printf("\nThe contents of the queu are as below.\n");
    node_t *cur = testqueue.front;
    while (cur != NULL) {
        process_t *currentProcess = (process_t *)cur->data;
        printf("The current process has identifier as %d, name is %s, cumulative run time is %ld, and priority is %d \n",
        currentProcess->identifier, currentProcess->name, currentProcess->runtime, currentProcess->priority);
        cur = cur->next;                        // Move the pointer to traverse the queue
    }
    printf("\n");


    // 4. Remove and return the process with highest priority
    process_t *priorityProcess = rmProcess(&testqueue);
    if (priorityProcess != NULL) {
        printf("The priority process has identifier as %d, name is %s, cumulative run time is %ld, and highest priority is %d \n", 
        priorityProcess->identifier, priorityProcess->name, priorityProcess->runtime, priorityProcess->priority);
    }
    printf("Queue size after removing the highest priority process: %d\n\n", qsize(&testqueue));

    return 0;
}