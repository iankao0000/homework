#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>       /* For O_* constants */
#include <string.h>

int main(int argc, char **argv)
{
    int fd = shm_open("posixsm", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, 0x400000);
    
    char *p = mmap(NULL, 0x400000,
                   PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    memset(p, 'A', 0x400000);
    munmap(p, 0x400000);
    
    return 0;
}
