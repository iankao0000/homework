#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef min
#define min(x, y) (x) < (y) ? (x) : (y)
#endif

#define N_THREADS 128
static int64_t loop_count = 0;

#pragma pack(1)
struct data {
    int32_t pad[15];
    int64_t v;
};

static struct data value __attribute__((aligned(64)));
static int64_t counter[N_THREADS];

static void worker(int *cnt) {
    for (int64_t i = 0; i < loop_count; ++i) {
        const int64_t t = value.v;

        if (t != 0L || t != ~0L)
            *cnt += 1;

        value.v = ~t;
        /* creates a compiler level memory barrier
         * forcing optimizer to not re-order memory
         * accesses across the barrier. */
        __asm__ volatile("" ::: "memory");
    }
}

int main(int argc, char *argv[]) {
    pthread_t threads[N_THREADS];

    if (argc != 3) {
        fprintf(stderr, "USAGE: %s <threads> <loopcount>\n", argv[0]);
        return 1;
    }

    /* Parse argument */
    int64_t n = min(atol(argv[1]), N_THREADS);
    loop_count = atol(argv[2]);

    /* Start the threads */
    for (int64_t i = 0L; i < n; ++i)
        pthread_create(&threads[i], NULL, (void *(*) (void *) ) worker,
                       &counter[i]);

    int64_t count = 0L;
    for (int64_t i = 0L; i < n; ++i) {
        pthread_join(threads[i], NULL);
        count += counter[i];
    }

    printf("iteration: %lu\n", count);
    printf("data size: %lu\n", sizeof(value));
    printf("final: %016lX\n", value.v);

    return 0;
}
