#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/types.h>
static void send_fd(int socket, int *fds, int n)
{
    char buf[CMSG_SPACE(n * sizeof(int))], data;
    memset(buf, '\0', sizeof(buf));
    struct iovec io = {.iov_base = &data, .iov_len = 1};

    struct msghdr msg = {.msg_iov = &io, .msg_iovlen = 1,
                         .msg_control = buf, .msg_controllen = sizeof(buf)};
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);                                                                                                               
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(n * sizeof(int));

    memcpy((int *) CMSG_DATA(cmsg), fds, n * sizeof(int));

    if (sendmsg(socket, &msg, 0) < 0)
        perror("Failed to send message");
}

int main(int argc, char *argv[])
{
    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) perror("Failed to create socket");

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path) - 1);

#define SIZE 0x400000
    int fds[2];
    fds[0] = memfd_create("shma", 0);
    if (fds[0] < 0)
        perror("Failed to open file 1 for reading");
    else
        fprintf(stdout, "Opened fd %d in parent\n", fds[0]);

    ftruncate(fds[0], SIZE);
    void *ptr0 =
        mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fds[0], 0);
    memset(ptr0, 'A', SIZE);
    munmap(ptr0, SIZE);

    fds[1] = memfd_create("shmb", 0);
    if (fds[1] < 0)
        perror("Failed to open file 2 for reading");
    else
        fprintf(stdout, "Opened fd %d in parent\n", fds[1]);
    ftruncate(fds[1], SIZE);
    void *ptr1 =
        mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fds[1], 0);
    memset(ptr1, 'B', SIZE);
    munmap(ptr1, SIZE);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) ==
        -1)
        perror("Failed to connect to socket");

    send_fd(sfd, fds, 2);

    exit(EXIT_SUCCESS);
}
