#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int
pidfd_open(pid_t pid, unsigned int flags)
{
    return syscall(SYS_pidfd_open, pid, flags);
}

static int
pidfd_getfd(int pidfd, int targetfd, unsigned int flags)
{
    return syscall(SYS_pidfd_getfd, pidfd, targetfd, flags);
}

int
main(int argc, char *argv[])
{
    int pidfd, sfd, ret;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <pid> <fd_num> <text>\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    printf("pidfd_open\n");
    pidfd = pidfd_open(atoi(argv[1]), 0);
    if (pidfd == -1) {
        perror("pidfd_open error");
        exit(EXIT_FAILURE);
    }

    printf("pidfd_getfd\n");
    sfd = pidfd_getfd(pidfd, atoi(argv[2]), 0);
    if (sfd == -1) {
        perror("pidfd_getfd error");
        exit(EXIT_FAILURE);
    }

    printf("write\n");
    ret = write(sfd, argv[3], strlen(argv[3]));
    if (ret == -1) {
        perror("write error");
        exit(EXIT_FAILURE);
    }

    close(sfd);
    close(pidfd);
    exit(EXIT_SUCCESS);
}
