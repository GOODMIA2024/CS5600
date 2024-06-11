#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define ARRAY_SIZE 5

int shared_array[ARRAY_SIZE];

void *worker(void *data)
{
    char *name = (char*)data;

    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        printf("Hi from thread name = %s\n", name);
    }

    printf("Thread %s done!\n", name);
    return NULL;
}

void *producer() {
    printf("Producer thread started\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        shared_array[i] = i * 10; // Adding some data to the shared array
        printf("Producer added: %d\n", shared_array[i]);
        usleep(100000); // Sleep for a while to simulate work
    }
    pthread_exit(NULL);
}

void *consumer() {
    printf("Consumer thread started\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int data = shared_array[i]; // Reading data from the shared array
        printf("Consumer read: %d\n", data);
        usleep(150000); // Sleep for a while to simulate work
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t th1, th2;
    pthread_create(&th1, NULL, worker, "X");
    pthread_create(&th2, NULL, worker, "Y");
    sleep(1);
    printf("====> Cancelling Thread %ld!!\n", (long)th2);
    pthread_cancel(th2);
    usleep(100000);
    printf("====> Cancelling Thread %ld!\n", (long)th1);
    pthread_cancel(th1);
    printf("exiting from main program\n");

    // threads communicate with each other

    /*
    Explanation:
    Although the code attempts to share data between threads using a global array,
    it may not exhibit consistent behavior due to the absence of synchronization mechanisms.
    Threads can access and modify shared data concurrently, leading to data races and inconsistencies.
    In this case, the producer thread might be writing to the shared array while the consumer thread
    is reading from it, resulting in unpredictable behavior.

    To avoid such problems and ensure data consistency, synchronization mechanisms such as mutexes,
    semaphores, or condition variables should be used to coordinate access to shared resources.
    Mutexes can be employed to ensure that only one thread can access the shared array at a time,
    preventing simultaneous reads and writes and maintaining data integrity.
    */

    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    printf("Exiting from main program\n");


    return 0;
}