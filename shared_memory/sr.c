#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    key_t key = ftok("/dev/shm/myshm", 0);
    int shm_id = shmget(key, 0x400000, 0666);
    char *p = (char *) shmat(shm_id, NULL, 0);
    
    printf("%c %c %c %c\n", p[0], p[1], p[2], p[3]);
    shmdt(p);
    
    return 0;
}
