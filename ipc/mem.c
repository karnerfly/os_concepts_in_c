#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 5
#define DATA_CYCLE 15

/* shared memory structure */
typedef struct
{
  int buffer[BUFF_SIZE];
  volatile int in;
  volatile int out;
  volatile int count;
} SharedMem;

int main()
{
  /* create shared memery with read write access */
  int shmid = shmget(IPC_PRIVATE, sizeof(SharedMem), S_IRUSR | S_IWUSR); // key, size in byte, mode (read|write)

  /* attach the memory with each processes address space */
  SharedMem *data = shmat(shmid, NULL, 0);

  /* initialize metadata of shared memory */
  data->in = 0;
  data->out = 0;
  data->count = 0;

  /* create new child process */
  pid_t pid = fork();

  if (pid < 0)
  {
    /* ditach shared memory from process address space (cleanup) */
    shmdt(data);

    /* clear the shared memory (cleanup) */
    shmctl(shmid, IPC_RMID, NULL);

    perror("fork() error\n");
    exit(1);
  }

  if (pid == 0)
  {
    /* consumer process each data item in the shared memory and waits if there is no items */
    for (size_t i = 0; i < DATA_CYCLE; i++)
    {
      while (data->count == 0)
        ;

      int item = data->buffer[data->out];
      printf("Consumer consumed: %d\n", item);
      data->out = (data->out + 1) % BUFF_SIZE;
      data->count--;

      sleep(1);
    }
  }
  else
  {
    /* producer produce data item in the shared memory and waits if the buffer is full */
    for (size_t i = 0; i < DATA_CYCLE; i++)
    {
      while (data->count == BUFF_SIZE)
        ;

      data->buffer[data->in] = i + 1;
      data->in = (data->in + 1) % BUFF_SIZE;
      data->count++;
    }
  }

  /* wait for all children to terminate */
  wait(NULL);

  /* ditach shared memory from process address space (cleanup) */
  shmdt(data);

  /* clear the shared memory (cleanup) */
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}
