#!/bin/sh

build=build

set -xe

rm -rf $build
mkdir -p $build

gcc -g -Wall -Wextra counter_unsafe.c -o $build/counter_unsafe
gcc -g -Wall -Wextra peterson_sol.c -o $build/peterson_sol
gcc -g -Wall -Wextra pthread_lock_sol.c -o $build/pthread_lock_sol
gcc -g -Wall -Wextra semaphore_sol.c -o $build/semaphore_sol
gcc -g -Wall -Wextra tsl_sol.c -o $build/tsl_sol

set +xe

echo "done."