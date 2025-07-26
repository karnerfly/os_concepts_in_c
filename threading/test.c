#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *print_message_thread1(void *param);
void *print_message_thread2(void *param);

int main(void)
{
  pthread_t tid1, tid2;
  int ret1, ret2;
  // pthread_attr_t attr1, attr2;
  char *msg1 = "hello";
  char *msg2 = "world";
  ret1 = pthread_create(&tid1, NULL, print_message_thread1, msg1);
  ret2 = pthread_create(&tid2, NULL, print_message_thread2, msg2);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  printf("Thread 1 returns: %d\n", ret1);
  printf("Thread 2 returns: %d\n", ret2);

  return 0;
}

void *print_message_thread1(void *param)
{
  char *msg = (char *)param;
  sleep(3);
  printf("Message: %s\n", msg);
  return NULL;
}

void *print_message_thread2(void *param)
{
  char *msg = (char *)param;
  sleep(4);
  printf("Message: %s\n", msg);
  return NULL;
}