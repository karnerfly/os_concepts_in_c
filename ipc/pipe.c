#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 512
#define READ_END 0
#define WRITE_END 1

int main()
{
  char buf[BUF_SIZE];
  char msg[BUF_SIZE] = "Hello World!";
  size_t msg_len = strlen(msg) + 1;
  int fd[2];
  pid_t child_id, parent_id;

  /* create ordinary pipe for communcate between two processes */
  if (pipe(fd) == -1)
  {
    perror("pipe() error\n");
    exit(1);
  }

  parent_id = getpid();
  child_id = fork();

  /* if fork failes then close the write and read end of the pipe (cleanup) */
  if (child_id < 0)
  {
    close(fd[READ_END]);
    close(fd[WRITE_END]);
    perror("fork() error\n");
    exit(1);
  }

  if (child_id == 0)
  {
    /* child close unused write end and read from read end and print the message */

    close(fd[WRITE_END]);
    read(fd[READ_END], buf, msg_len);
    close(fd[READ_END]);

    printf("MSG: %s\n", buf);
  }
  else
  {
    /* parent close unused read end and write to write end and print details */

    close(fd[READ_END]);
    write(fd[WRITE_END], msg, msg_len);
    close(fd[WRITE_END]);

    printf("Message sent successfully :: parentId=%d, childId=%d\n", parent_id, child_id);
  }

  wait(NULL);
  return 0;
}