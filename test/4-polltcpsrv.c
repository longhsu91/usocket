#include "uns_socket.h"
#include "uns_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>
#include <limits.h>

#define OPEN_MAX 20

int main(int argc, char **argv)
{
    int listen_fd = 0;
    int connect_fd = 0;
    struct sockaddr_in srv_addr;
    char buf[MAXLINE];
    int n;

    listen_fd = uns_socket(AF_INET, SOCK_STREAM, 0);
    bzero(&srv_addr, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    srv_addr.sin_port = htons(5000);

    uns_bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

    uns_listen(listen_fd, 5);

    // poll
    struct pollfd client[OPEN_MAX];
    int nready = 0;
    int max_fd = 0;
    int socket = -1;

    client[0].fd = listen_fd;
    client[0].events = POLLRDNORM;

    for (int i = 1; i < OPEN_MAX; i++)
    {
        client[i].fd = -1;
    }

    max_fd = 0;

    while (1)
    {
        uns_print("wait for event...");
        nready = poll(client, max_fd + 1, -1);
        uns_print("get %d event from client", nready);

        // new connect
        if (client[0].revents & POLLRDNORM)
        {
            if ((connect_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL)) < 0)
            {
                // signal will interrupt the accept call
                if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    uns_error_exit("accept error: %s", strerror(errno));
                }
            }

            uns_print("new client %d connected...", connect_fd);

            int i = 1;
            for (i = 1; i < OPEN_MAX; i++)
            {
                if (client[i].fd < 0)
                {
                    client[i].fd = connect_fd;
                    break;
                }
            }

            client[i].events = POLLRDNORM;
            max_fd = max_fd > connect_fd ? max_fd : connect_fd;

            if (--nready <= 0)
            {
                continue;
            }
        }

        for (int i = 1; i <= max_fd; i++)
        {
            socket = client[i].fd;
            // check client for recv data
            if (socket < 0)
            {
                continue;
            }

            if (client[i].revents & (POLLRDNORM | POLLERR))
            {
                uns_print("new msg arrive on %d", socket);
                if ((n = recv(socket, buf, MAXLINE, 0)) > 0)
                {
                    uns_send(socket, buf, MAXLINE, 0);
                }
                else if (n < 0)
                {
                    // connnect reset by client
                    if (errno == ECONNRESET)
                    {
                        uns_close(socket);
                        client[i].fd = -1;
                    }
                }
                else
                {
                    // connect close by client
                    uns_close(socket);
                    client[i].fd = -1;
                }

                if (--nready <= 0)
                {
                    break;
                }
            }
        }
    }

    uns_close(listen_fd);

    return 0;
}