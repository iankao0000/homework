#include <sys/mman.h>
#include <fcntl.h> // for shm_unlink
#include <stdio.h> // for printf

int main() {
    if (shm_unlink("posixsm") == -1) {
        perror("shm_unlink");
        return 1;
    }
    printf("Shared memory 'posixsm' released successfully.\n");
    return 0;
}

