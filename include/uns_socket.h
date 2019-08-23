
#ifndef _UNS_SOCKET_H_INCLUDE_
#define _UNS_SOCKET_H_INCLUDE_

#include <sys/socket.h>

#define MAXLINE 1024

int uns_socket(int family, int type, int protocal);

void uns_bind(int fd, const struct sockaddr *addr, socklen_t len);

void uns_listen(int fd, int backlog);

int uns_accept(int fd, struct sockaddr *addr, socklen_t *len);

void uns_connect(int fd, const struct sockaddr *addr, socklen_t len);

void uns_close(int fd);

void uns_send(int fd, const void *ptr, size_t nbytes, int flags);

int uns_recv(int fd, void *ptr, size_t nbytes, int flags);

#endif