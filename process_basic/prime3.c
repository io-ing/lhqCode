#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT 3000000
#define RIGHT 3000200

#define NUM_PROCESSES 3     /* 假设我们设置的进程数为 4 */

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
    int j;
    pid_t pid;
    int status;

    for (i=0; i<NUM_PROCESSES; ++i)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(1);
        }

        if (pid == 0)
        {
            for (j=LEFT+i; j<=RIGHT; j+=NUM_PROCESSES)
            {
                if (is_primer(j))
                {
                    printf("Process %d found prime number: %d\n", i, j);
                }
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
