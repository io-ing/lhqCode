#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 1024

int main()
{
    int pipefd[2];
    pid_t pid;
    char buf[BUFSIZE];


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
        read(pipefd[0], buf, sizeof(buf));
        puts(buf);


        close(pipefd[0]);
        exit(0);
    }
    else
    {
        close(pipefd[0]);
        write(pipefd[1], "hello", 5);
        wait(NULL);
        close(pipefd[1]);
        exit(0);
    }
}
