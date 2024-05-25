#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    printf("begin\n");  /* 注意：没有换行符 */
    fflush(NULL);       /* 非常重要 */

    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        /* 子进程执行的代码 */
        printf("Child process is running with PID: %d\n", getpid());
    }
    else
    {
        /* 父进程执行的代码 */
        printf("Parent process is running with PID: %d\n", getpid());
        sleep(1000);
    }
    printf("end\n");

//    getchar();
    exit(0);
}
