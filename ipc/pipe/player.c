#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFSIZE 1024

int main()
{
    int pipefd[2];
    pid_t pid;
    char buf[BUFSIZE];
    int len;

    if (pipe(pipefd) < 0)
    {
        perror("pipe()");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        perror("fork()");
        exit(1);
    }

    if (pid == 0)
    {
        close(pipefd[1]);
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        fd = open("/dev/null", O_RDWR);
        dup2(fd, 1);
        dup2(fd, 2);
        execl("/usr/bin/mpg123", "mpg123", "-", NULL);
        perror("execl()");
        exit(1);
    }
    else
    {
        close(pipefd[0]);
        /* 父进程从网上收数据，王管道中写 */
        close(pipefd[1]);
        wait(NULL);
        exit(0);
    }
}
