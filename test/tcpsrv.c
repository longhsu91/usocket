#include "uns_socket.h"
#include "uns_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static void sig_child(int sig_number)
{
    uns_print("catch signal %d from child", sig_number);

    pid_t pid;
    int status;

    while (1)
    {
        pid = waitpid(-1, &status, WNOHANG);

        if (pid > 0)
        {
            uns_print("child process %d has exited", pid);
        }
        else
        {
            break;
        }
    }
}

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

    // set singal to handle child process
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = sig_child;
    sigaction(SIGCHLD, &sa, NULL);

    while (1)
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

        uns_print("new client connected...");

        pid_t child_pid = fork();

        if (child_pid == 0)
        {
            uns_close(listen_fd);
            while ((n = uns_recv(connect_fd, buf, MAXLINE, 0)) > 0)
            {
                uns_send(connect_fd, buf, MAXLINE, 0);
            }

            if (n == 0)
            {
                uns_close(connect_fd);
                exit(0);
            }
        }

        uns_close(connect_fd);
    }

    uns_close(listen_fd);
}