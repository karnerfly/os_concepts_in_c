#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sched.h>

size_t counter = 0;
size_t turn = 0;
bool interest[2] = {false, false};

void *inc_counter(void *p);
void *dec_counter(void *p);

void entry_section(size_t pid);
void exit_section(size_t pid);

int main(void)
{
  pthread_t tid1, tid2;
  int ret1, ret2;

  ret1 = pthread_create(&tid1, NULL, inc_counter, (void *)0);
  ret2 = pthread_create(&tid2, NULL, dec_counter, (void *)1);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("Thread 1 returns: %d, Thread 2 returns: %d\n", ret1, ret2);
  printf("Final Value: %ld\n", counter);

  return 0;
}

void *inc_counter(void *p)
{
  size_t pid = (size_t)p;

  for (size_t i = 0; i < 1000000; i++)
  {
    entry_section(pid);
    /* critical section start */
    counter++;
    /* critical section end */
    exit_section(pid);
  }
  return NULL;
}

void *dec_counter(void *p)
{
  size_t pid = (size_t)p;

  for (size_t i = 0; i < 1000000; i++)
  {
    entry_section(pid);
    /* critical section start */
    counter--;
    /* critical section end */
    exit_section(pid);
  }
  return NULL;
}

void entry_section(size_t pid)
{
  size_t other_pid = 1 - pid;
  interest[pid] = true;
  turn = other_pid;
  while (interest[other_pid] && turn == other_pid)
  {
    if (sched_yield() == -1)
    {
      fprintf(stderr, "falied to reschedule the thread with pid %ld\n", pid);
      exit(EXIT_FAILURE);
    }
  }
}

void exit_section(size_t pid)
{
  interest[pid] = false;
}