#!/bin/sh

set -xe

find . -type f ! -name "*.*" -delete

gcc -g -Wall -Wextra counter_unsafe.c -o counter_unsafe
gcc -g -Wall -Wextra peterson_sol.c -o peterson_sol
gcc -g -Wall -Wextra pthread_lock_sol.c -o pthread_lock_sol
gcc -g -Wall -Wextra semaphore_sol.c -o semaphore_sol
gcc -g -Wall -Wextra my_semaphore_sol.c -o my_semaphore_sol

set +xe

echo "done."