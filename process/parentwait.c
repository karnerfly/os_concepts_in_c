#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  pid_t pid;

  pid = fork();

  // fork syscall failed
  if (pid < 0) {
    fprintf(stderr, "Fork failed");
    return 1;
  }

  // child process
  if (pid == 0){
   execlp("./child", "", NULL); 
  }

  wait(NULL);
  printf("child complete id=%d\n", pid);
  return 0;
}
