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


static int *recv_fd(int socket, int n)
{
    int *fds = malloc(n * sizeof(int));
    char buf[CMSG_SPACE(n * sizeof(int))], data;
    memset(buf, '\0', sizeof(buf));
    struct iovec io = {.iov_base=&data, .iov_len=1};

    struct msghdr msg = {.msg_iov = &io, .msg_iovlen = 1,
                         .msg_control = buf, .msg_controllen = sizeof(buf)};
    if (recvmsg(socket, &msg, 0) < 0)
        perror("Failed to receive message");

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    memcpy(fds, (int *) CMSG_DATA(cmsg), n * sizeof(int));

    return fds;
}




int main(int argc, char *argv[])
{
    char buffer[256];

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) perror("Failed to create socket");

    if (unlink("/tmp/fd-pass.socket") == -1 && errno != ENOENT)
        perror("Removing socket file failed");

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
        perror("Failed to bind to socket");

    if (listen(sfd, 5) == -1) perror("Failed to listen on socket");

    int cfd = accept(sfd, NULL, NULL);
    if (cfd == -1) perror("failed to accept incoming connection");

    int *fds = recv_fd(cfd, 2);

    for (int i = 0; i < 2; ++i) {
        fprintf(stdout, "Reading from passed fd %d\n", fds[i]);
        ssize_t nbytes;
        while ((nbytes = read(fds[i], buffer, sizeof(buffer))) > 0)
            write(1, buffer, nbytes);
        *buffer = '\0';
    }

    if (close(cfd) == -1) perror("Failed to close client socket");

    return 0;
}
