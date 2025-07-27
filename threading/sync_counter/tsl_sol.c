#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

int counter = 0;
atomic_flag lock = ATOMIC_FLAG_INIT;

void *inc_counter(void *p);
void *dec_counter(void *p);

bool tsl();
void tsl_release();

int main(void)
{
  pthread_t tid1, tid2;
  int ret1, ret2;

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
    tsl();
    /* critical section start */
    counter++;
    /* critical section end */
    tsl_release();
  }
  return NULL;
}

void *dec_counter(void *p)
{
  (void)p;
  for (size_t i = 0; i < 1000000; i++)
  {
    tsl();
    /* critical section start */
    counter--;
    /* critical section end */
    tsl_release();
  }
  return NULL;
}

bool tsl()
{
  return atomic_flag_test_and_set_explicit(&lock, memory_order_seq_cst);
}

void tsl_release()
{
  atomic_flag_clear_explicit(&lock, memory_order_release);
}