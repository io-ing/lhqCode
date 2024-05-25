#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT 3000000
#define RIGHT 3000200

int is_primer(int num)
{
    int i;

    for (i=2; i<num; ++i)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    int i;
    pid_t pid;
    int status;

    for (i=LEFT; i<RIGHT; ++i)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("pid()");
            exit(1);
        }

        if (pid == 0)
        {
            if (is_primer(i))
            {
                printf("%d is primer\n", i);
            }
            exit(0);    /* 子进程计算完成后退出 */
        }
        /* 父进程继续循环创建子进程 */
    }

    while ((pid = wait(&status)) > 0)
    {

        if (WIFEXITED(status))
        {
            printf("Child %d terminated with exit status %d\n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child %d terminated by signal %d\n", pid, WTERMSIG(status));
        }
    }

    exit(0);
}
