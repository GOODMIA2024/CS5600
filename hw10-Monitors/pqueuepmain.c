#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 15 // Increase buffer capacity to 15

int buffer[BUF_SIZE];						
int num = 0;
int totalProduced = 0; // Track total items produced
int totalConsumed = 0; // Track total items consumed									
pthread_mutex_t mut;	
pthread_cond_t cond; 

void *producer(void *param);
void *consumer(void *param);

main (int argc, char *argv[])
{
  pthread_t t1, t2, t3, t4; // Additional threads
  // Initialize mutex and condition variable
  pthread_mutex_init(&mut, NULL);
  pthread_cond_init(&cond, NULL);

    // Create two producer and two consumer threads
    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, producer, NULL); // Second producer thread
    pthread_create(&t3, NULL, consumer, NULL);
    pthread_create(&t4, NULL, consumer, NULL); // Second consumer thread

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    printf("All threads completed.\n");

    // Clean up
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    return 0;
}
void *producer(void *param)
{
    int i = 0;
    while(1) {
        pthread_mutex_lock(&mut);
        while (num == BUF_SIZE)          
            pthread_cond_wait(&cond, &mut);
        if (totalProduced >= 30) { // Check if 30 items have been produced
            printf("30 items produced, producer exiting.\n");
            pthread_mutex_unlock(&mut);
            return NULL; // Exit the thread
        }
        buffer[num++] = i;
        totalProduced++;
        printf("Produced: %d, Buffer Size: %d, Total Produced: %d\n", i++, num, totalProduced);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mut);
        sleep(1); // Simulate work
    }
}

void *consumer(void *param)
{
    while (1) {
        pthread_mutex_lock(&mut);
        while (num == 0)       
            pthread_cond_wait(&cond, &mut);
        if (totalConsumed >= 30) { // Check if 30 items have been consumed
            printf("30 items consumed, consumer exiting.\n");
            pthread_mutex_unlock(&mut);
            return NULL; // Exit the thread
        }
        int i = buffer[--num]; // Consume item
        totalConsumed++;
        printf("Consumed: %d, Buffer Size: %d, Total Consumed: %d\n", i, num, totalConsumed);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mut);
        sleep(3); // Simulate work
    }
}