#include "uns_socket.h"
#include "uns_error.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <strings.h>
#include <arpa/inet.h>

#define LOCAL_ADDRESS "127.0.0.1"

int main(int argc, char **argv)
{
    int socket_fd = 0;
    struct sockaddr_in srv_addr;
    char buf[MAXLINE];
    int n;

    socket_fd = uns_socket(AF_INET, SOCK_STREAM, 0);

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

    uns_connect(socket_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

    while ((n = uns_recv(socket_fd, buf, MAXLINE, 0)) > 0)
    {
        buf[n] = 0;
        if (fputs(buf, stdout) == EOF)
        {
            uns_error_exit("fputs error.");
        }
    }

    if (n < 0)
    {
        uns_error_exit("recv error.");
    }

    return 0;
}