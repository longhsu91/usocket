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

    // select
    fd_set read_sets;
    fd_set tmp_sets;
    FD_ZERO(&read_sets);
    FD_SET(listen_fd, &read_sets);
    int max_fd = listen_fd + 1;
    tmp_sets = read_sets;

    while (1)
    {
        read_sets = tmp_sets;

        uns_print("wait for event...");
        select(max_fd, &read_sets, NULL, NULL, NULL);

        for (int fd = 0; fd < max_fd; fd++)
        {
            // new connect
            if (fd == listen_fd && FD_ISSET(listen_fd, &read_sets))
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

                if (max_fd < (connect_fd + 1))
                {
                    max_fd = connect_fd + 1;
                }

                FD_SET(connect_fd, &tmp_sets);

                continue;
            }

            if (FD_ISSET(fd, &read_sets))
            {
                uns_print("new msg arrive on %d", fd);
                if ((n = uns_recv(fd, buf, MAXLINE, 0)) > 0)
                {
                    uns_send(fd, buf, MAXLINE, 0);
                }

                if (n == 0)
                {
                    uns_close(fd);
                    FD_CLR(fd, &tmp_sets);
                }
            }
        }
    }

    uns_close(listen_fd);

    return 0;
}