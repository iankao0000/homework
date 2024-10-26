#include <stdio.h>                         
#include <unistd.h>

#include "queue.h"

#define BUFFER_SIZE (getpagesize())
#define NUM_THREADS (4)
size_t MESSAGES_PER_THREAD = 0;
#define PARSE_ARGUMENT(arg) (MESSAGES_PER_THREAD = strtoul(arg, NULL, 10))


uint64_t get_time()
{
    struct timespec ts;
    clock_gettime(0, &ts);
    return (uint64_t)(ts.tv_sec * 1e6 + ts.tv_nsec / 1e3);
}



static void *consumer_loop(void *arg)
{
    queue_t *q = (queue_t *) arg;
    size_t count = 0;
    for (size_t i = 0; i < MESSAGES_PER_THREAD; i++) {
        size_t x;
        queue_get(q, (uint8_t *) &x, sizeof(size_t));
        count++;
//	printf("%ld\n", i);
    }
    return (void *) count;
}

static void *publisher_loop(void *arg)
{
    queue_t *q = (queue_t *) arg;
    size_t i;
    for (i = 0; i < NUM_THREADS * MESSAGES_PER_THREAD; i++)
        queue_put(q, (uint8_t *) &i, sizeof(size_t));
    return (void *) i;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }
    PARSE_ARGUMENT(argv[1]);

    queue_t q;
    queue_init(&q, BUFFER_SIZE);

    uint64_t start = get_time();

    pthread_t publisher;
    pthread_t consumers[NUM_THREADS];

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&publisher, &attr, &publisher_loop, (void *) &q);

    for (intptr_t i = 0; i < NUM_THREADS; i++)
        pthread_create(&consumers[i], &attr, &consumer_loop, (void *) &q);

    intptr_t sent;
    pthread_join(publisher, (void **) &sent);
//    printf("publisher sent %ld messages\n", sent);

    intptr_t recd[NUM_THREADS];
    for (intptr_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(consumers[i], (void **) &recd[i]);
//        printf("consumer %ld received %ld messages\n", i, recd[i]);
    }

    uint64_t end = get_time();

    printf("%ld\t", MESSAGES_PER_THREAD>>7);

    printf("%ld\n", end - start);

    pthread_attr_destroy(&attr);

    queue_destroy(&q);

    return 0;
}
