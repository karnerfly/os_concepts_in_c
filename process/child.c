#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
  sleep(2);
  printf("child process 1\n");
  return 0;
}
