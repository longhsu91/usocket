#include "uns_socket.h"
#include "uns_error.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <strings.h>
#include <arpa/inet.h>

#define LOCAL_ADDRESS "127.0.0.1"

#define SOCKET_COUNT 1

int main(int argc, char **argv)
{
    int socket_fds[10];
    struct sockaddr_in srv_addr;
    char buf[MAXLINE];
    int n;

    for (int i = 0; i < SOCKET_COUNT; i++)
    {
        socket_fds[i] = uns_socket(AF_INET, SOCK_STREAM, 0);

        bzero(&srv_addr, sizeof(srv_addr));
        srv_addr.sin_family = AF_INET;
        srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        srv_addr.sin_port = htons(5000);

        char addr_buf[MAXLINE];
        memcpy(addr_buf, argc == 2 ? argv[1] : LOCAL_ADDRESS, strlen(argc == 2 ? argv[1] : LOCAL_ADDRESS) + 1);
        if (inet_pton(AF_INET, addr_buf, &srv_addr.sin_addr) <= 0)
        {
            uns_print("inet_pton error");
        }

        uns_connect(socket_fds[i], (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    }

    while (fgets(buf, MAXLINE, stdin) != NULL)
    {
        /* contains value close */
        if (strcasestr(buf, "close") != NULL)
        {
            break;
        }

        uns_send(socket_fds[0], buf, MAXLINE, 0);

        uns_recv(socket_fds[0], buf, MAXLINE, 0);

        fputs(buf, stdout);
    }

    // exit will close all fds
    // uns_close(socket_fd);

    return 0;
}