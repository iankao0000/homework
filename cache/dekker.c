
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static volatile int flag1 = 0, flag2 = 0, turn = 1;
static volatile int counter = 0;
int loop_cnt;

static void dekker1(void)
{
    flag1 = 1;
    turn = 2;
    // __atomic_thread_fence(__ATOMIC_SEQ_CST);
    while ((flag2 == 1) && (turn == 2))
        ;
    /* critical section */
    counter++;
    /* let the other task run */
    flag1 = 0;
}

static void dekker2(void)
{
    flag2 = 1;
    turn = 1;
    // __atomic_thread_fence(__ATOMIC_SEQ_CST);
    while ((flag1 == 1) && (turn == 1))
        ;
    /* critical section */
    counter++;
    /* leave critical section */
    flag2 = 0;
}

static void *task1(void *arg)
{
    printf("Starting %s\n", __func__);
    for (int i = loop_cnt; i > 0; i--)
        dekker1();
    return NULL;
}

static void *task2(void *arg)
{
    printf("Starting %s\n", __func__);
    for (int i = loop_cnt; i > 0; i--)
        dekker2();
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t thread1, thread2;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <loopcount>\n", argv[0]);
        exit(1);
    }

    loop_cnt = atoi(argv[1]); /* FIXME: format checks */
    int expected_sum = 2 * loop_cnt;

    (void) pthread_create(&thread1, NULL, task1, NULL);
    (void) pthread_create(&thread2, NULL, task2, NULL);

    void *ret;
    (void) pthread_join(thread1, &ret);
    (void) pthread_join(thread2, &ret);
    printf("Both threads terminated\n");

    /* Check result */
    if (counter != expected_sum) {
        printf("[-] Dekker did not work, sum %d rather than %d.\n", counter,
               expected_sum);
        printf("%d missed updates due to memory consistency races.\n",
               (expected_sum - counter));
        return 1;
    }
    printf("[+] Dekker worked.\n");
    return 0;
}
