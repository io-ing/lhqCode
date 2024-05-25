#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(void)
{
    /* 获取环境变量数组的起始地址 */
    char **env = environ;
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }

    exit(0);
}
