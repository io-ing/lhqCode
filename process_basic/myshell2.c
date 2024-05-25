#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <glob.h>

static void prompt()
{
    printf("myshell-0.1$ ");
    fflush(stdout);
}

#if 0
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
#else
int parse(char *line, glob_t *result)
{
    char *token;
    char delim[] = " \t\n";
    int i = 0;

    while(1)
    {
        token = strsep(&line, delim);
        if (token == NULL)
            break;
        if (token[0] == '\0')
            continue;
        
        glob(token, GLOB_NOCHECK|GLOB_APPEND*i, NULL, result);
        i = 1;
    }

    return 0;
}
#endif

int main()
{
    char *line = NULL;
    size_t line_size = 0;
    pid_t pid;
    glob_t result;

    while (1)
    {
        prompt();
        if (getline(&line, &line_size, stdin) < 0)
        {
            break; // 读取失败或 EOF 结束循环
        }

        if (parse(line, &result) != 0)
        {
            continue; // 解析失败则跳过
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(1);
        }
        
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
            waitpid(pid, NULL, 0);
        }
        else
        {
            perror("fork failed");
        }
        globfree(&result); // 释放 glob 结构分配的内存
    }
    free(line);

    exit(0);
}
