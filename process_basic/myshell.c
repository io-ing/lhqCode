#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <glob.h>

void prompt()
{
    printf("myshell-0.1$ ");
    fflush(stdout);
}

int parse(char *line, glob_t *result)
{
    char *token;
    char delim[] = " \t\n";
    int i = 0;

    while ((token = strtok(i==0 ? line : NULL, delim)) != NULL)
    {
        glob(token, i++ ? GLOB_APPEND : 0, NULL, result);
    }

    return 0;
}

int main()
{
    char *line = NULL;
    size_t line_size = 0;
    glob_t result;

    while (1)
    {
        prompt();
        if (getline(&line, &line_size, stdin) == -1)
        {
            break; // 读取失败或 EOF 结束循环
        }
        if (parse(line, &result) != 0)
        {
            continue; // 解析失败则跳过
        }
        pid_t pid = fork();
        if (pid == 0)
        {
            // 子进程执行外部命令
            execvp(result.gl_pathv[0], result.gl_pathv);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0)
        {
            // 父进程等待子进程结束
            int status;
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork failed");
        }
        globfree(&result); // 释放 glob 结构分配的内存
    }
    free(line);

    return 0;
}
