
#include "uns_socket.h"
#include "uns_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int uns_socket(int family, int type, int protocal)
{
    int n;

    if ((n = socket(family, type, protocal)) < 0)
    {
        uns_error_exit("socket error");
    }

    return n;
}

void uns_bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    if (bind(fd, addr, len) < 0)
    {
        uns_error_exit("socket bind error");
    }
}

void uns_listen(int fd, int backlog)
{
    if (listen(fd, backlog) < 0)
    {
        uns_error_exit("socket listen error");
    }
}

int uns_accept(int fd, struct sockaddr *addr, socklen_t *len)
{
    int n;

    if ((n = accept(fd, addr, len)) < 0)
    {
        uns_error_exit("socket accept error");
    }

    return n;
}

void uns_connect(int fd, const struct sockaddr *addr, socklen_t len)
{
    if (connect(fd, addr, len) < 0)
    {
        uns_error_exit("socket connect error");
    }
}

void uns_close(int fd)
{
    if (close(fd) < 0)
    {
        uns_error_exit("socket close error");
    }
}

/**************************************************************/

void uns_send(int fd, const void *ptr, size_t nbytes, int flags)
{
    if (send(fd, ptr, nbytes, flags) != (ssize_t)nbytes)
    {
        uns_error_exit("send error");
    }
}

int uns_recv(int fd, void *ptr, size_t nbytes, int flags)
{
    ssize_t n;

    if ((n = recv(fd, ptr, nbytes, flags)) < 0)
    {
        uns_error_exit("recv error");
    }

    return n;
}