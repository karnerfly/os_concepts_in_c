#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t mutex;

void *inc_counter(void *p);
void *dec_counter(void *p);

int main(void)
{
  pthread_t tid1, tid2;
  int ret1, ret2;

  pthread_mutex_init(&mutex, NULL);

  ret1 = pthread_create(&tid1, NULL, inc_counter, NULL);
  ret2 = pthread_create(&tid2, NULL, dec_counter, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("Thread 1 returns: %d, Thread 2 returns: %d\n", ret1, ret2);
  printf("Final Value: %d\n", counter);

  return 0;
}

void *inc_counter(void *p)
{
  (void)p;
  for (size_t i = 0; i < 1000000; i++)
  {
    pthread_mutex_lock(&mutex);
    /* critical section start */
    counter++;
    /* critical section end */
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void *dec_counter(void *p)
{
  (void)p;
  for (size_t i = 0; i < 1000000; i++)
  {
    pthread_mutex_lock(&mutex);
    /* critical section start */
    counter--;
    /* critical section end */
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}