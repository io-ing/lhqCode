#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    exit(0);
}
