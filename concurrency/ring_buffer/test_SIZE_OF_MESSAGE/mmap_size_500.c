#include <stdio.h>                         
#include <unistd.h>
#define SIZE_OF_MESSAGE 500ULL

#include "queue.h"

size_t in[65536];
size_t out[65536];

#define BUFFER_SIZE (getpagesize())
#define NUM_THREADS (1)
size_t MESSAGES_PER_THREAD = 0;
#define PARSE_ARGUMENT(arg) (MESSAGES_PER_THREAD = strtoul(arg, NULL, 10))


uint64_t get_time()
{
    struct timespec ts;
    clock_gettime(0, &ts);
    return (uint64_t)(ts.tv_sec * 1e6 + ts.tv_nsec / 1e3);
}


static void *publisher_loop(void *arg)
{
    queue_t *q = (queue_t *) arg;
//    rbuf_t *r = (rbuf_t *) arg;
    size_t i;
    size_t **publisher_ptr = malloc(sizeof(size_t *));
    *publisher_ptr = in;
    size_t full_put = (NUM_THREADS * MESSAGES_PER_THREAD) / SIZE_OF_MESSAGE;
    size_t remain_put = (NUM_THREADS * MESSAGES_PER_THREAD) % SIZE_OF_MESSAGE;
//    size_t full_put = (r->messages_per_thread * r->num_threads) / SIZE_OF_MESSAGE;
//    size_t remain_put = (r->messages_per_thread * r->num_threads) % SIZE_OF_MESSAGE;
    for (i = 0; i < full_put; i++)
        queue_put(q, (uint8_t **) publisher_ptr, sizeof(size_t) * SIZE_OF_MESSAGE);
//        queue_put(&r->q, (uint8_t **) publisher_ptr, sizeof(size_t) * SIZE_OF_MESSAGE);
    if(remain_put)
        queue_put(q, (uint8_t **) publisher_ptr, sizeof(size_t) * remain_put);
//        queue_put(&r->q, (uint8_t **) publisher_ptr, sizeof(size_t) * remain_put);
    return (void *) (i * SIZE_OF_MESSAGE + remain_put);
}

static void *consumer_loop(void *arg)
{
    queue_t *q = (queue_t *) arg;
//    rbuf_t *r = (rbuf_t *) arg;
    size_t i;
    size_t **consumer_ptr = malloc(sizeof(size_t *));
    *consumer_ptr = out;
    size_t full_get = MESSAGES_PER_THREAD / SIZE_OF_MESSAGE;
    size_t remain_get = MESSAGES_PER_THREAD % SIZE_OF_MESSAGE;
//    size_t full_get = (r->messages_per_thread) / SIZE_OF_MESSAGE;
//    size_t remain_get = (r->messages_per_thread) % SIZE_OF_MESSAGE;
    for (i = 0; i < full_get; i++)
        queue_get(q, (uint8_t **) consumer_ptr, sizeof(size_t) * SIZE_OF_MESSAGE);
//        queue_get(&r->q, (uint8_t **) consumer_ptr, sizeof(size_t) * SIZE_OF_MESSAGE);
    if(remain_get)
	queue_get(q, (uint8_t **) consumer_ptr, sizeof(size_t) * remain_get);
//        queue_get(&r->q, (uint8_t **) consumer_ptr, sizeof(size_t) * remain_get);
    return (void *) (i * SIZE_OF_MESSAGE + remain_get);
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
