#include <stdio.h>
#include <string.h>
#include <pthread.h>

char *error = NULL;

int has_error(int ret)
{
  return !!ret;
}

int valid_username_length(char *name)
{
  int len = strlen(name);
  if (len < 4)
  {
    return 0;
  }
  return 1;
}

int valid_username_format(char *name)
{
  char *invalid_chars = "#$^&*(){}[]<>,/\\?'\"|`~!%";
  for (size_t i = 0; name[i] != '\0'; i++)
  {
    for (size_t j = 0; invalid_chars[j] != '\0'; j++)
    {
      if (name[i] == invalid_chars[j])
      {
        return 0;
      }
    }
  }
  return 1;
}

void *check_valid_username(void *__vpname)
{
  char *name = (char *)__vpname;

  if (!valid_username_length(name))
  {
    error = "invalid username length";
    pthread_exit(NULL);
  }

  if (!valid_username_format(name))
  {
    error = "invalid username format";
    pthread_exit(NULL);
  }

  // error = NULL;
  pthread_exit(NULL);
}

void *print_error(void *p)
{
  (void)p;
  while (1)
  {
    pthread_testcancel();
    while (error == NULL)
    {
      pthread_testcancel();
    }
    fprintf(stderr, "error: %s\n", error);
    error = NULL;
  }
}

pthread_t spawn_validation_thread(char *name)
{
  pthread_t tid;
  pthread_attr_t attr;

  if (has_error(pthread_attr_init(&attr)))
  {
    fprintf(stderr, "failed to initialize thread attributes\n");
    return 1;
  }

  size_t stack_size = 1 << 20;
  if (has_error(pthread_attr_setstacksize(&attr, stack_size)))
  {
    fprintf(stderr, "failed to set stack size in thread attributes\n");
    return 2;
  }

  if (has_error(pthread_create(&tid, &attr, check_valid_username, (void *)name)))
  {
    fprintf(stderr, "failed to create thread\n");
    return 3;
  }

  pthread_attr_destroy(&attr);
  return tid;
}

pthread_t spawn_error_print_thread()
{
  pthread_t tid;
  pthread_attr_t attr;

  if (has_error(pthread_attr_init(&attr)))
  {
    fprintf(stderr, "failed to initialize thread attributes\n");
    return 1;
  }

  size_t stack_size = 512 << 10;
  if (has_error(pthread_attr_setstacksize(&attr, stack_size)))
  {
    fprintf(stderr, "failed to set stack size in thread attributes\n");
    return 2;
  }

  if (has_error(pthread_create(&tid, &attr, print_error, NULL)))
  {
    fprintf(stderr, "failed to create thread\n");
    return 3;
  }

  pthread_attr_destroy(&attr);
  return tid;
}

int main()
{
  char name[100];
  pthread_t error_thread_id = spawn_error_print_thread();
  while (1)
  {
    printf("Enter Name: ");
    fgets(name, 50, stdin);
    if (strlen(name) == 2 && name[0] == 'q')
    {
      pthread_cancel(error_thread_id);
      pthread_join(error_thread_id, NULL);
      break;
    }
    pthread_t validation_thread_id = spawn_validation_thread(name);
    pthread_join(validation_thread_id, NULL);
  }
  return 0;
}
