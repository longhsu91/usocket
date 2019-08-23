#include "uns_socket.h"

#include <stdio.h>
#include <time.h>
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <strings.h>

int main(int argc, char **argv)
{
    int listen_fd = 0;
    int connect_fd = 0;
    struct sockaddr_in srv_addr;
    char buf[MAXLINE];
    time_t times;

    listen_fd = uns_socket(AF_INET, SOCK_STREAM, 0);
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(5000);

    uns_bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

    uns_listen(listen_fd, 5);

    while (1)
    {
        uns_print("accept new client...");

        connect_fd = uns_accept(listen_fd, (struct sockaddr *)NULL, NULL);

        times = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&times));

        uns_send(connect_fd, buf, sizeof(buf), 0);

        uns_close(connect_fd);
    }
}