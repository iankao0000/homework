#include <pthread.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#if defined(__x86_64__) || defined(__i386__)
#define CPU_PAUSE() asm("pause")
#elif defined(__aarch64__)
#define CPU_PAUSE() asm("isb")
#else
#define CPU_PAUSE() do { /* nop */ } while (0)
#endif

struct atomic_float {
    volatile atomic_flag flag;
    int padding; /* avoid false sharing */
    alignas(16) float value;
};

static volatile struct atomic_float atomic_float_obj;

static void atomic_val_inc(int nLoops)
{
    for (int loop = 0; loop < nLoops; loop++) {
//        while (atomic_flag_test_and_set(&atomic_float_obj.flag))
//            CPU_PAUSE();
        atomic_float_obj.value += 1.0f;
        atomic_flag_clear(&atomic_float_obj.flag);
    }
}

static void *func(void *args)
{
    atomic_val_inc(10000);
    return NULL;
}

int main(int argc, const char *argv[])
{
    printf("The size is: %zu, `value` offset is: %zu\n",
           sizeof(atomic_float_obj), offsetof(struct atomic_float, value));

    atomic_float_obj.flag = (atomic_flag) ATOMIC_FLAG_INIT;
    atomic_float_obj.value = 0.0f;

    pthread_t tid;
    if (pthread_create(&tid, NULL, func, NULL) != 0) {
        puts("Failed to create a thread!");
        return 1;
    }

    atomic_val_inc(10000);

    pthread_join(tid, NULL);
    printf("The final result is: %f\n", atomic_float_obj.value);
    return 0;
}
