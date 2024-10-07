#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static int loop_count;
static int A, B, X, Y;

static void *worker1(void *arg)
{
    X = 1;
    asm volatile("" ::: "memory");
    A = Y;
}

static void *worker2(void *arg)
{
    Y = 1;
    asm volatile("" ::: "memory");
    B = X;
}

int main(int argc, char *argv[])
{
    pthread_t thread_1, thread_2;

    int64_t count = 0;

    if (argc != 2) {
        fprintf(stderr, "USAGE: %s <loop_count>\n", argv[0]);
        exit(1);
    }

    loop_count = atoi(argv[1]); /* FIXME: format checks */

    for (int i = 0; i < loop_count; ++i) {
        X = 0;
        Y = 0;

        /* Start the threads */
        pthread_create(&thread_1, NULL, (void *(*) (void *) ) worker1, NULL);
        pthread_create(&thread_2, NULL, (void *(*) (void *) ) worker2, NULL);

        /* Wait for the threads to end */
        pthread_join(thread_1, NULL);
        pthread_join(thread_2, NULL);

        if (A == 0 && B == 0) /* reorder caught */
            count++;
    }

    printf("%ld reorder caught in %d iterations.\n", count, loop_count);

    return 0;
}
