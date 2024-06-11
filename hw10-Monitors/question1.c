#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 10

int buffer[BUF_SIZE];
int num = 0;
pthread_mutex_t mut;
pthread_cond_t cond;

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[])
{
  pthread_t t1, t2;

  if(pthread_mutex_init(&mut, NULL) != 0) {
    perror("pthread_mutex_init");
    exit(1);
  }
  if(pthread_cond_init(&cond, NULL) != 0) {
    perror("pthread_cond_init");
    exit(1);
  }
  if (pthread_create(&t1, NULL, producer, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }
  if (pthread_create(&t2, NULL, consumer, NULL) != 0) {
    perror("pthread_create");
    exit(1);
  }

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("WE ARE DONE\n");

  pthread_mutex_destroy(&mut);
  pthread_cond_destroy(&cond);

  return 0;
}

void *producer(void *param)
{
  int i = 0;
  while(1) {
    pthread_mutex_lock(&mut);
    if (num > BUF_SIZE) exit(1); 
    while (num == BUF_SIZE)          
      pthread_cond_wait(&cond, &mut);
    buffer[num] = i;
    printf("Produced: %d, Buffer Size: %d\n", i, num + 1);
    num++;
    i++;
    pthread_mutex_unlock(&mut);
    pthread_cond_signal(&cond);
    sleep(1);
  }
}

void *consumer(void *param)
{
  int i;
  while (1) {
    pthread_mutex_lock(&mut);
    if (num < 0) exit(1);
    while (num == 0)       
      pthread_cond_wait(&cond, &mut);
    i = buffer[--num]; // Access the correct item then decrement num
    printf("Consumed: %d, Buffer Size: %d\n", i, num);
    pthread_mutex_unlock(&mut);
    pthread_cond_signal(&cond);
    sleep(3);
  }
}
